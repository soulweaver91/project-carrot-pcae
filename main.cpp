
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <memory>
#include <QtCore/QCoreApplication>
#include <QString>
#include <QDataStream>
#include <QFile>
#include <QPair>
#include <QByteArray>
#include <QBitArray>
#include <QVector>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <palette.h>
#include <IDMapping.h>

const std::string PCAE_VERSION = "0.9.7";

QByteArray BEfromLE(QByteArray le) {
    QDataStream a(le);
    QByteArray be;
    a.setByteOrder(QDataStream::BigEndian);
    qint8 byte;
    while (!a.atEnd()) {
        a >> byte;
        be.prepend(byte);
    }
    return be;
}

quint32 uintFromArray(const QByteArray& array) {
    if (array.size() != 4) {
        return 0;
    }
    QDataStream a(BEfromLE(array));
    quint32 res = 0;
    quint8 byte;
    while (!a.atEnd()) {
        res *= 256;
        a >> byte;
        res += byte;
    }
    return res;
}

quint16 ushortFromArray(const QByteArray& array) {
    if (array.size() != 2) {
        return 0;
    }
    QDataStream a(BEfromLE(array));
    quint32 res = 0;
    quint8 byte;
    while (!a.atEnd()) {
        res *= 256;
        a >> byte;
        res += byte;
    }
    return res;
}

quint8 ucharFromArray(const QByteArray& array) {
    if (array.size() != 1) {
        return 0;
    }
    QDataStream a(array);
    quint8 byte;
    a >> byte;
    return byte;
}

QByteArray arrayFromUshort(quint16 val) {
    QByteArray a;
    a.append((quint8)(val & 0x00FF));
    a.append((quint8)((val & 0xFF00) >> 8));

    return a;
}

QByteArray arrayFromUint(quint32 val) {
    QByteArray a;
    a.append((quint8)(val & 0x000000FF));
    a.append((quint8)((val & 0x0000FF00) >> 8));
    a.append((quint8)((val & 0x00FF0000) >> 16));
    a.append((quint8)((val & 0xFF000000) >> 24));

    return a;
}

struct J2AnimFrame {
    QPair<quint16, quint16> size;
    QPair<qint16, qint16> coldspot;
    QPair<qint16, qint16> hotspot;
    QPair<qint16, qint16> gunspot;
    QByteArray imageData;
    QBitArray maskData;
    qint32 imageAddr;
    qint32 maskAddr;
    bool drawTransparent;
};

struct J2Anim {
    quint16 frameCnt;
    quint16 fps;
    QVector<J2AnimFrame> frames;
    quint16 set;
    quint16 anim;
    QPair<qint16, qint16> adjustedSize;
    QPair<qint16, qint16> largestOffset;
    QPair<qint16, qint16> normalizedHotspot;
    QPair<qint16, qint16> frameConfiguration;
};

struct J2Sample {
    quint16 id;
    quint16 set;
    quint16 idInSet;
    quint32 sampleRate;
    QByteArray soundData;
    quint16 multiplier;
};

QByteArray readBytes(QByteArray& src, quint32 n) {
    QByteArray head = src.left(n);
    src.remove(0, n);

    return head;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "PCAE version " << PCAE_VERSION << "\n"
            << "Usage: pcae path\n\n"
            << "Extracts resources from inside an Anims.j2a file to be used with Project Carrot.\n\n"
            << "Parameters:\n"
            << "  path         Required. Specifies where the Anims.j2a file is located.\n";
        return EXIT_FAILURE;
    }

    QString animsLocation(argv[1]);

    QFile fh(animsLocation);
    if (!(fh.exists())) {
        std::cerr << "ERROR: The file \"" << animsLocation.toLocal8Bit().data() << "\" cannot be found!\n";
        return EXIT_FAILURE;
    }
    if (!(fh.open(QIODevice::ReadOnly))) {
        std::cerr << "ERROR: Cannot open file \"" << animsLocation.toLocal8Bit().data() << "\"!\n";
        return EXIT_FAILURE;
    }
    std::cout << "Opening file \"" << animsLocation.toLocal8Bit().data() << "\"...\n";

    qint32 filesize = fh.size();

    try {
        bool seemsLikeCC = false;

        qint32 magicALIB = uintFromArray(fh.read(4));
        qint32 magicBABE = uintFromArray(fh.read(4));

        // Magic headers
        if (magicALIB != 0x42494C41 || magicBABE != 0x00BEBA00) {
            std::cerr << "ERROR: This doesn't look like an Anims.j2a file! (invalid header)\n";
            return EXIT_FAILURE;
        }

        // 1.20  JJ2 vanilla, 1.23 JJ2 patched: 464 bytes
        // 1.23x JJ2 HH: 476 bytes
        // 1.24  JJ2 TSF: 500 bytes
        qint32 headerLen = uintFromArray(fh.read(4));

        qint32 magicUnknown = uintFromArray(fh.read(4));
        if (magicUnknown != 0x18080200) {
            std::cerr << "ERROR: This doesn't look like an Anims.j2a file! (invalid header)\n";
            return EXIT_FAILURE;
        }

        qint32 fileLen = uintFromArray(fh.read(4));
        qint32 crc = uintFromArray(fh.read(4));
        qint32 setCnt = uintFromArray(fh.read(4));
        QVector<qint32> setAddresses;
        setAddresses.reserve(setCnt);

        for (quint32 i = 0; i < setCnt; ++i) {
            setAddresses.append(uintFromArray(fh.read(4)));
        }

        if (headerLen != fh.pos()) {
            std::cerr << "ERROR: Header length doesn't seem to match the set number!\n"
                      << "Expected header of length " << headerLen << " bytes, got " << fh.pos() << ".\n";
            return EXIT_FAILURE;
        }

        QVector<std::shared_ptr<J2Anim>> anims;
        QVector<J2Sample> samples;

        for (quint32 i = 0; i < setCnt; ++i) {
            std::cout << "Parsing set " << i << "...\n";

            qint32 magicANIM = uintFromArray(fh.read(4));
            quint8 animCount = ucharFromArray(fh.read(1));
            quint8 sndCount = ucharFromArray(fh.read(1));
            quint16 frameCount = ushortFromArray(fh.read(2));
            quint32 cumulativeSndIndex = uintFromArray(fh.read(4));
            quint32 infoBlockLenC = uintFromArray(fh.read(4));
            QByteArray infoBlockLenU = BEfromLE(fh.read(4));
            quint32 frameDataBlockLenC = uintFromArray(fh.read(4));
            QByteArray frameDataBlockLenU = BEfromLE(fh.read(4));
            quint32 imageDataBlockLenC = uintFromArray(fh.read(4));
            QByteArray imageDataBlockLenU = BEfromLE(fh.read(4));
            quint32 sampleDataBlockLenC = uintFromArray(fh.read(4));
            QByteArray sampleDataBlockLenU = BEfromLE(fh.read(4));

            std::cout << "Set " << i << " has "
                      << frameCount << " frames in "
                      << (quint16)animCount << " animations, and "
                      << (quint16)sndCount << " samples.\n";

            QByteArray infoBlock       = qUncompress(infoBlockLenU       + fh.read(infoBlockLenC));
            QByteArray frameDataBlock  = qUncompress(frameDataBlockLenU  + fh.read(frameDataBlockLenC));
            QByteArray imageDataBlock  = qUncompress(imageDataBlockLenU  + fh.read(imageDataBlockLenC));
            QByteArray sampleDataBlock = qUncompress(sampleDataBlockLenU + fh.read(sampleDataBlockLenC));

            if (magicANIM != 0x4D494E41) {
                std::cerr << "WARNING: Header for set " << i << " is incorrect (bad magic value)!\n"
                          << "Skipping the subfile.\n";
                continue;
            }

            if (infoBlock.size() == 0 && uintFromArray(infoBlockLenU) != 0) {
                std::cerr << "ERROR: Unpacking the information block failed!\n";
                throw 0;
            }
            if (frameDataBlock.size() == 0 && uintFromArray(frameDataBlockLenU) != 0) {
                std::cerr << "ERROR: Unpacking the frame data block failed!\n";
                throw 0;
            }
            if (imageDataBlock.size() == 0 && uintFromArray(imageDataBlockLenU) != 0) {
                std::cerr << "ERROR: Unpacking the image data block failed!\n";
                throw 0;
            }
            if (sampleDataBlock.size() == 0 && uintFromArray(sampleDataBlockLenU) != 0) {
                std::cerr << sampleDataBlockLenC << ", " << uintFromArray(sampleDataBlockLenU) << "\n";
                std::cerr << "ERROR: Unpacking the sample data block failed!\n";
                throw 0;
            }

            QVector<std::shared_ptr<J2Anim>> setAnims;

            for (quint16 j = 0; j < animCount; ++j) {
                std::shared_ptr<J2Anim> anim = std::make_shared<J2Anim>();
                anim->set = i;
                anim->anim = j;
                anim->frameCnt = ushortFromArray(readBytes(infoBlock, 2));
                anim->fps = ushortFromArray(readBytes(infoBlock, 2));
                anim->normalizedHotspot = qMakePair(0, 0);
                anim->adjustedSize = qMakePair(0, 0);
                std::cout << "  Set=" << i << " anim=" << j << ": "
                          << anim->frameCnt << " frames, " << anim->fps << "FPS\n";

                // Skip the rest, seems to be 0x00000000 for all headers
                // Docs state this is used internally in the exe for... something
                infoBlock.remove(0, 4);

                anims.append(anim);
                setAnims.append(anim);
            }

            if (i == 65 && setAnims.length() > 5) {
                seemsLikeCC = true;
            }

            if (frameCount > 0) {
                if (setAnims.length() == 0) {
                    std::cerr << "ERROR: Set has frames but no anims!\n";
                    throw 0;
                }

                QPair<qint16, qint16> lastColdspot;
                QPair<qint16, qint16> lastGunspot;
                QPair<qint16, qint16> lastHotspot;

                std::shared_ptr<J2Anim> currentAnim = setAnims.at(0);
                quint16 currentAnimIdx = 0;
                quint16 currentFrame = 0;
                for (quint16 j = 0; j < frameCount; j++) {
                    if (currentFrame >= currentAnim->frameCnt) {
                        currentAnim = setAnims.at(++currentAnimIdx);
                        currentFrame = 0;
                    }

                    J2AnimFrame frame;
                    quint16 x, y;

                    x = ushortFromArray(readBytes(frameDataBlock, 2));
                    y = ushortFromArray(readBytes(frameDataBlock, 2));
                    frame.size = qMakePair(x, y);

                    x = (qint16)ushortFromArray(readBytes(frameDataBlock, 2));
                    y = (qint16)ushortFromArray(readBytes(frameDataBlock, 2));
                    frame.coldspot = qMakePair(x, y);

                    x = (qint16)ushortFromArray(readBytes(frameDataBlock, 2));
                    y = (qint16)ushortFromArray(readBytes(frameDataBlock, 2));
                    frame.hotspot = qMakePair(x, y);

                    x = (qint16)ushortFromArray(readBytes(frameDataBlock, 2));
                    y = (qint16)ushortFromArray(readBytes(frameDataBlock, 2));
                    frame.gunspot = qMakePair(x, y);

                    frame.imageAddr = uintFromArray(readBytes(frameDataBlock, 4));
                    frame.maskAddr = uintFromArray(readBytes(frameDataBlock, 4));

                    // Adjust normalized position
                    // In the output images, we want to make the hotspot and image size constant.
                    currentAnim->normalizedHotspot.first  = std::max(qint16(-frame.hotspot.first),  currentAnim->normalizedHotspot.first);
                    currentAnim->normalizedHotspot.second = std::max(qint16(-frame.hotspot.second), currentAnim->normalizedHotspot.second);
                    currentAnim->largestOffset.first  = std::max(qint16(frame.size.first  + frame.hotspot.first),  currentAnim->largestOffset.first);
                    currentAnim->largestOffset.second = std::max(qint16(frame.size.second + frame.hotspot.second), currentAnim->largestOffset.second);
                    currentAnim->adjustedSize.first = std::max(
                        qint16(currentAnim->normalizedHotspot.first + currentAnim->largestOffset.first),
                        currentAnim->adjustedSize.first
                        );
                    currentAnim->adjustedSize.second = std::max(
                        qint16(currentAnim->normalizedHotspot.second + currentAnim->largestOffset.second),
                        currentAnim->adjustedSize.second
                        );

                    currentAnim->frames.append(frame);


                    if (currentFrame > 0) {
                        QPair<qint16, qint16> diffNew;
                        QPair<qint16, qint16> diffOld;

                        if (frame.coldspot != qMakePair<qint16, qint16>(0, 0)) {
                            diffNew = qMakePair<qint16, qint16>(frame.coldspot.first - frame.hotspot.first, frame.coldspot.second - frame.hotspot.second);
                            diffOld = qMakePair<qint16, qint16>(lastColdspot.first - lastHotspot.first, lastColdspot.second - lastHotspot.second);
                            if (diffNew != diffOld) {
                                std::cerr << "WARNING: Animation coldspots don't agree for set " << currentAnim->set << " anim " << currentAnim->anim << "!\n";
                                std::cerr << "F" << (currentFrame - 1) << ": " << diffOld.first << "," << diffOld.second << ", ";
                                std::cerr << "F" << currentFrame << ": " << diffNew.first << "," << diffNew.second << "\n";
                            }
                        }

                        if (frame.gunspot != qMakePair<qint16, qint16>(0, 0)) {
                            diffNew = qMakePair<qint16, qint16>(frame.gunspot.first - frame.hotspot.first, frame.gunspot.second - frame.hotspot.second);
                            diffOld = qMakePair<qint16, qint16>(lastGunspot.first - lastHotspot.first, lastGunspot.second - lastHotspot.second);
                            if (diffNew != diffOld) {
                                std::cerr << "WARNING: Animation gunspots don't agree for set " << currentAnim->set << " anim " << currentAnim->anim << "!\n";
                                std::cerr << "F" << (currentFrame - 1) << ": " << diffOld.first << "," << diffOld.second << ", ";
                                std::cerr << "F" << currentFrame << ": " << diffNew.first << "," << diffNew.second << "\n";
                            }
                        }
                    }
                    lastColdspot = frame.coldspot;
                    lastGunspot = frame.gunspot;
                    lastHotspot = frame.hotspot;

                    currentFrame++;
                }

                // Read the image data for each animation frame
                for (quint16 j = 0; j < setAnims.length(); ++j) {
                    std::shared_ptr<J2Anim> anim = setAnims.at(j);

                    if (anim->frameCnt < anim->frames.length()) {
                        std::cerr << "ERROR: Set " << anim->set << " anim " << anim->anim << " frame count doesn't match!\n";
                        std::cerr << "Expected " << anim->frameCnt << " frames but read " << anim->frames.length() << "\n";
                        throw 0;
                    }

                    for (quint16 frame = 0; frame < anim->frameCnt; ++frame) {
                        QByteArray* imageData = &anim->frames[frame].imageData;
                        quint32 dpos = anim->frames.at(frame).imageAddr + 4;

                        quint16 width2 = ushortFromArray(imageDataBlock.mid(dpos - 4, 2));
                        quint16 height2 = ushortFromArray(imageDataBlock.mid(dpos - 2, 2));
                        anim->frames[frame].drawTransparent = (width2 & 0x8000) > 0;

                        quint16 pxRead = 0;
                        quint16 pxTotal = anim->frames.at(frame).size.first * anim->frames.at(frame).size.second;
                        bool lastOpEmpty = true;

                        while (pxRead < pxTotal) {
                            if (dpos > 0x10000000) {
                                std::cerr << "WARNING: loading image data probably failed! Aborting.\n";
                                break;
                            }
                            quint8 op = ucharFromArray(imageDataBlock.mid(dpos, 1));
                            if (op == 0) {
                                //std::cerr << "WARNING: next image operation should probably not be 0x00\n";
                            }
                            
                            if (op < 0x80) {
                                // Skip the given number of pixels, writing them with the transparent color 0
                                pxRead += op;
                                while (op-- > 0) {
                                    imageData->append((char) 0x00);
                                }
                                dpos++;
                            } else if (op == 0x80) {
                                // Skip until the end of the line
                                quint16 linePxLeft = anim->frames.at(frame).size.first - pxRead % anim->frames.at(frame).size.first;
                                if (pxRead % anim->frames.at(frame).size.first == 0 && !lastOpEmpty) {
                                    linePxLeft = 0;
                                }

                                pxRead += linePxLeft;
                                while (linePxLeft-- > 0) {
                                    imageData->append((char) 0x00);
                                }
                                dpos++;
                            } else {
                                // Copy specified amount of pixels (ignoring the high bit)
                                quint16 bytesToRead = op & 0x7F;
                                QByteArray nextData = imageDataBlock.mid(dpos + 1, bytesToRead);
                                imageData->append(nextData);
                                pxRead += bytesToRead;
                                dpos += bytesToRead + 1;
                            }

                            lastOpEmpty = (op == 0x80);
                        }

                        QBitArray* maskData = &anim->frames[frame].maskData;
                        maskData->fill(false, pxTotal);
                        dpos = anim->frames.at(frame).maskAddr;
                        pxRead = 0;

                        // no mask
                        if (dpos == 0xFFFFFFFF) {
                            continue;
                        }

                        while (pxRead < pxTotal) {
                            quint8 byte = ucharFromArray(imageDataBlock.mid(dpos, 1));
                            for (quint8 bit = 0; bit < 8 && (pxRead + bit) < pxTotal; ++bit) {
                                maskData->setBit(pxRead + bit, byte & (1 << (7 - bit)));
                            }
                            pxRead += 8;
                        }
                    }
                }
            } else {

            }

            for (quint16 j = 0; j < sndCount; ++j) {
                J2Sample sample;
                sample.id = cumulativeSndIndex + j;
                sample.idInSet = j;
                sample.set = i;

                quint32 totalSize = uintFromArray(readBytes(sampleDataBlock, 4));
                quint32 magicRIFF = uintFromArray(readBytes(sampleDataBlock, 4));
                quint32 chunkSize = uintFromArray(readBytes(sampleDataBlock, 4));
                // "ASFF" for 1.20, "AS  " for 1.24
                quint32 format = uintFromArray(readBytes(sampleDataBlock, 4));
                bool isASFF = format == 0x46465341;

                quint32 magicSAMP = uintFromArray(readBytes(sampleDataBlock, 4));
                quint32 sampSize = uintFromArray(readBytes(sampleDataBlock, 4));
                // Padding/unknown data #1
                // For set 0 sample 0:
                //       1.20                           1.24
                //  +00  00 00 00 00 00 00 00 00   +00  40 00 00 00 00 00 00 00
                //  +08  00 00 00 00 00 00 00 00   +08  00 00 00 00 00 00 00 00
                //  +10  00 00 00 00 00 00 00 00   +10  00 00 00 00 00 00 00 00
                //  +18  00 00 00 00               +18  00 00 00 00 00 00 00 00
                //                                 +20  00 00 00 00 00 40 FF 7F
                sampleDataBlock.remove(0, 40 - (isASFF ? 12 : 0));
                if (isASFF) {
                    // All 1.20 samples seem to be 8-bit. Some of them are among those
                    // for which 1.24 reads as 24-bit but that might just be a mistake.
                    sampleDataBlock.remove(0, 2);
                    sample.multiplier = 0;
                } else {
                    // for 1.24. 1.20 has "20 40" instead in s0s0 which makes no sense
                    sample.multiplier = ushortFromArray(readBytes(sampleDataBlock, 2));
                }
                // Unknown. s0s0 1.20: 00 80, 1.24: 80 00
                sampleDataBlock.remove(0, 2);
                quint32 payloadSize = uintFromArray(readBytes(sampleDataBlock, 4));
                // Padding #2, all zeroes in both
                sampleDataBlock.remove(0, 8);
                sample.sampleRate = uintFromArray(readBytes(sampleDataBlock, 4));
                quint32 actualDataSize = chunkSize - 76 + (isASFF ? 12 : 0);

                sample.soundData = readBytes(sampleDataBlock, actualDataSize);
                // Padding #3
                sampleDataBlock.remove(0, 4);

                if (magicRIFF != 0x46464952 || magicSAMP != 0x504D4153) {
                    std::cerr << "ERROR: Sample has invalid header!\n";
                    throw 0;
                }


                if (sample.soundData.length() < actualDataSize) {
                    std::cerr << "WARNING: Sample was shorter than expected! (Expected " 
                              << actualDataSize << " bytes, only read " << sample.soundData.length() << ")\n";
                }

                std::cout << "  Set=" << i << " samp=" << j << ": " << actualDataSize << " bytes, "
                          << (payloadSize * 1000 / sample.sampleRate) << "ms @" << sample.sampleRate << "Hz\n";

                if (totalSize > chunkSize + 12) {
                    // Sample data is probably aligned to X bytes since the next sample doesn't always appear
                    // right after the first ends.
                    std::cout << "Adjusted read offset by " << (totalSize - chunkSize - 12) << " byte(s).\n";
                    sampleDataBlock.remove(0, totalSize - chunkSize - 12);
                }

                samples.append(sample);
            }
        }

        fh.close();

        uint version;
        if (headerLen == 464) {
            version = JJ2Version::ORIGINAL;
            std::cout << "Detected Jazz Jackrabbit 2 version 1.20/1.23.\n";
        } else if (headerLen == 500 && seemsLikeCC) {
            version = JJ2Version::CC;
            std::cout << "Detected Jazz Jackrabbit 2: Christmas Chronicles.\n";
        } else if (headerLen == 500 && !seemsLikeCC) {
            version = JJ2Version::TSF;
            std::cout << "Detected Jazz Jackrabbit 2: The Secret Files.\n";
        } else if (headerLen == 476) {
            version = JJ2Version::HH;
            std::cout << "Detected Jazz Jackrabbit 2: Holiday Hare '98.\n";
        } else {
            version = JJ2Version::UNKNOWN;
            std::cout << "Could not determine the version.\n";
        }

        auto mapper = std::make_unique<IDMapper>(version);

        std::unique_ptr<AnimIDMap> animMapping = mapper->getAnimMapping();
        std::unique_ptr<SampleIDMap> sampleMapping = mapper->getSampleMapping();

        QDir outdir(QDir::current());
        // If fails, likely means it already existed
        // TODO: if another error occurred, handle it
        outdir.mkdir("Assets");
        outdir.cd("Assets");


        // Process the extracted data next
        for (quint32 i = 0; i < anims.length(); ++i) {
            std::shared_ptr<J2Anim> currentAnim = anims.at(i);
            AnimMapping mappingForAnim = animMapping->value(qMakePair(currentAnim->set, currentAnim->anim), IDMapper::EMPTY_ANIM_MAPPING);

            // Determine the frame configuration to use.
            // Each asset must fit into a 4096 by 4096 texture,
            // as that is the smallest texture size we have decided to support.
            currentAnim->frameConfiguration.first = currentAnim->frameCnt;
            currentAnim->frameConfiguration.second = 1;
            if (currentAnim->frameCnt > 1) {
                currentAnim->frameConfiguration.second = std::max(1, (int)std::ceil(std::sqrt(currentAnim->frameCnt * currentAnim->adjustedSize.first / currentAnim->adjustedSize.second)));
                currentAnim->frameConfiguration.first = std::max(1, (int)std::ceil(currentAnim->frameCnt * 1.0 / currentAnim->frameConfiguration.second));

                // Do a bit of optimization, as the above algorithm ends occasionally with some extra space
                // (it is careful with not underestimating the required space)
                while (currentAnim->frameConfiguration.first * (currentAnim->frameConfiguration.second - 1) >= currentAnim->frameCnt) {
                    currentAnim->frameConfiguration.second--;
                }
            }

            sf::Image img;
            img.create(currentAnim->adjustedSize.first * currentAnim->frameConfiguration.first,
                        currentAnim->adjustedSize.second * currentAnim->frameConfiguration.second, sf::Color(0, 0, 0, 0));

            for (int j = 0; j < currentAnim->frames.length(); ++j) {
                J2AnimFrame* frame = &currentAnim->frames[j];
                quint32 offsetX = currentAnim->normalizedHotspot.first  + frame->hotspot.first;
                quint32 offsetY = currentAnim->normalizedHotspot.second + frame->hotspot.second;

                for (quint16 y = 0; y < frame->size.second; ++y) {
                    for (quint16 x = 0; x < frame->size.first; ++x) {
                        quint16 targetX = (j % currentAnim->frameConfiguration.first) * currentAnim->adjustedSize.first + offsetX + x;
                        quint16 targetY = (j / currentAnim->frameConfiguration.first) * currentAnim->adjustedSize.second + offsetY + y;
                        quint8 colorIdx = frame->imageData.at(frame->size.first * y + x);
                        sf::Color color = mappingForAnim.palette[colorIdx];
                        if (frame->drawTransparent) {
                            color.a = std::min(sf::Uint8(127), color.a);
                        }

                        img.setPixel(targetX, targetY, color);
                    }
                }
            }

            QString filename;
            if (mappingForAnim.name == "") {
                filename = "s" + QString::number(currentAnim->set) + "_a" + QString::number(currentAnim->anim) + ".png";
            } else {
                if (!outdir.exists(mappingForAnim.category)) {
                    outdir.mkdir(mappingForAnim.category);
                }
                filename = mappingForAnim.category + "/" + mappingForAnim.name + ".png";
            }
            
            std::cout << "Saving set " << (quint16)currentAnim->set << " anim " << currentAnim->anim << "\n";
            img.saveToFile(outdir.absoluteFilePath(filename).toStdString());

            QJsonObject meta;
            meta.insert("description", "Set " + QString::number(currentAnim->set) + " anim " + QString::number(currentAnim->anim));
            meta.insert("generator", "Project Carrot Anim Extractor v" + QString::fromStdString(PCAE_VERSION));

            QString verString;
            switch (version) {
                case JJ2Version::ORIGINAL: verString = "1.20/.23"; break;
                case JJ2Version::TSF:      verString = "1.24"; break;
                default:                   verString = "unknown";
            }

            meta.insert("animsVersion", verString);

            QJsonObject root;
            root.insert("width", QJsonValue(currentAnim->adjustedSize.first));
            root.insert("height", QJsonValue(currentAnim->adjustedSize.second));
            root.insert("framesPerRow", QJsonValue(currentAnim->frameConfiguration.first));
            root.insert("framesPerCol", QJsonValue(currentAnim->frameConfiguration.second));
            root.insert("frameCount", QJsonValue(currentAnim->frameCnt));
            root.insert("hotspot", QJsonValue(QJsonArray({ currentAnim->normalizedHotspot.first, currentAnim->normalizedHotspot.second })));
            if (currentAnim->frames[0].coldspot.first == 0 && currentAnim->frames[0].coldspot.second == 0) {
                root.insert("coldspot", QJsonValue::Null);
            } else {
                root.insert("coldspot", QJsonValue(QJsonArray({
                    (currentAnim->normalizedHotspot.first + currentAnim->frames[0].hotspot.first) - currentAnim->frames[0].coldspot.first,
                    (currentAnim->normalizedHotspot.second + currentAnim->frames[0].hotspot.second) - currentAnim->frames[0].coldspot.second
                })));
            }

            if (currentAnim->frames[0].gunspot.first == 0 && currentAnim->frames[0].gunspot.second == 0) {
                root.insert("gunspot", QJsonValue::Null);
            } else {
                root.insert("gunspot", QJsonValue(QJsonArray({
                    (currentAnim->normalizedHotspot.first + currentAnim->frames[0].hotspot.first) - currentAnim->frames[0].gunspot.first,
                    (currentAnim->normalizedHotspot.second + currentAnim->frames[0].hotspot.second) - currentAnim->frames[0].gunspot.second
                })));
            }

            root.insert("fps", QJsonValue(currentAnim->fps));
            root.insert("scaling", QJsonValue(1));
            root.insert("meta", meta);

            QJsonDocument doc;
            doc.setObject(root);

            QFile jsonfh(outdir.absoluteFilePath(filename + ".json"));
            if (!(jsonfh.open(QIODevice::WriteOnly))) {
                std::cerr << "WARNING: Cannot write metadata file for image file " << filename.toStdString() << "!\n";
                continue;
            }
            jsonfh.write(doc.toJson());
            jsonfh.close();
        }

        for (quint32 i = 0; i < samples.length(); ++i) {
            //const J2Sample* sample = &samples.at(i);
            J2Sample* sample = &samples[i];

            std::cout << "Saving set " << sample->set << " sample " << sample->idInSet << "\n";

            SampleMapping mappingForSample = sampleMapping->value(qMakePair(sample->set, sample->idInSet), IDMapper::EMPTY_SAMPLE_MAPPING);
            QString filename;
            if (mappingForSample.name == "") {
                filename = "s" + QString::number(sample->set) + "_s" + QString::number(sample->idInSet) + ".wav";
            } else {
                if (!outdir.exists(mappingForSample.category)) {
                    outdir.mkdir(mappingForSample.category);
                }
                filename = mappingForSample.category + "/" + mappingForSample.name + ".wav";
            }

            QFile waveFile(outdir.absoluteFilePath(filename));
            if (!(waveFile.open(QIODevice::WriteOnly))) {
                std::cerr << "WARNING: Cannot open file " << filename.toStdString() << " for writing!\n";
                continue;
            }
            // If overwriting, make sure nothing is left behind in case the new version is shorter.
            waveFile.resize(0);

            // TODO: The modulo here essentially clips the sample to 8- or 16-bit.
            // There are some samples (at least the Rapier random noise) that at least get reported as 24-bit
            // by the read header data. It is not clear if they actually are or if the header data is just
            // read incorrectly, though - one would think the data would need to be reshaped between 24 and 8
            // but it works just fine as is.
            quint16 multiplier = (sample->multiplier / 4) % 2 + 1;

            // Main header
            waveFile.write("RIFF");
            waveFile.write(arrayFromUint(sample->soundData.length() + 36));
            waveFile.write("WAVE");

            // Format header
            waveFile.write("fmt ");
            waveFile.write(arrayFromUint(16)); // header remainder length
            waveFile.write(arrayFromUshort(1)); // format = PCM
            waveFile.write(arrayFromUshort(1)); // channels
            waveFile.write(arrayFromUint(sample->sampleRate)); // sample rate
            waveFile.write(arrayFromUint(sample->sampleRate * multiplier)); // byte rate
            waveFile.write(arrayFromUint(multiplier * 0x00080001));

            // Payload
            waveFile.write("data");
            waveFile.write(arrayFromUint(sample->soundData.length())); // payload length
            while (sample->soundData.length() > 0) {
                waveFile.write(QByteArray(1, (char)((multiplier << 7) ^ ucharFromArray(readBytes(sample->soundData, 1)))));
            }
            //waveFile.write(sample->soundData);

            waveFile.close();
        }

        std::cout << "\nDone.\nPress Enter to continue...\n";
    } catch (...) {
        std::cout << "Aborting extraction...\n";
        if (fh.isOpen()) {
            fh.close();
        }
        getchar();
        return EXIT_FAILURE;
    }
    getchar();
    return EXIT_SUCCESS;
}
