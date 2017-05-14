#include "Jazz2AnimSet.h"
#include "Jazz2FormatParseException.h"

Jazz2AnimSet::Jazz2AnimSet(quint16 number, Jazz2FormatDataBlock& data) : number(number) {
    qint32 magicANIM = data.readUInt();

    if (magicANIM != 0x4D494E41) {
        throw Jazz2FormatParseException(INVALID_MAGIC);
    }

    quint8 animCount = data.readChar();
    quint8 sampleCount = data.readChar();
    quint16 frameCount = data.readUShort();
    quint32 cumulativeSampleIndex = data.readUInt();
    
    // Read the lengths, uncompress the blocks and bail if any block could not be uncompressed
    // This could look better without all the copy-paste, but meh.
    quint32 infoBlockPackedSize,   frameDataBlockPackedSize,   imageDataBlockPackedSize,   sampleDataBlockPackedSize,
            infoBlockUnpackedSize, frameDataBlockUnpackedSize, imageDataBlockUnpackedSize, sampleDataBlockUnpackedSize;
    infoBlockPackedSize         = data.readUInt();
    infoBlockUnpackedSize       = data.readUInt();
    frameDataBlockPackedSize    = data.readUInt();
    frameDataBlockUnpackedSize  = data.readUInt();
    imageDataBlockPackedSize    = data.readUInt();
    imageDataBlockUnpackedSize  = data.readUInt();
    sampleDataBlockPackedSize   = data.readUInt();
    sampleDataBlockUnpackedSize = data.readUInt();

    Jazz2FormatDataBlock infoBlock      (data.readRawBytes(infoBlockPackedSize      ), true, infoBlockUnpackedSize      , 0);
    Jazz2FormatDataBlock frameDataBlock (data.readRawBytes(frameDataBlockPackedSize ), true, frameDataBlockUnpackedSize , 1);
    Jazz2FormatDataBlock imageDataBlock (data.readRawBytes(imageDataBlockPackedSize ), true, imageDataBlockUnpackedSize , 2);
    Jazz2FormatDataBlock sampleDataBlock(data.readRawBytes(sampleDataBlockPackedSize), true, sampleDataBlockUnpackedSize, 3);
    QByteArray* rawImageData = imageDataBlock.getRawData();

    animations.reserve(animCount);
    audioSamples.reserve(sampleCount);

    readAnimations(animCount, infoBlock, frameDataBlock, rawImageData);
    readSamples(sampleCount, sampleDataBlock);
}

void Jazz2AnimSet::writeAssetsToRawFiles(const QDir& outDir, std::shared_ptr<AnimIDMap> animMapping, std::shared_ptr<SampleIDMap> sampleMapping, Jazz2AnimVersion version, std::ostream& progressOutput) {
    for (auto& anim : animations) {
        AnimMapping mappingForAnim = IDMapper::EMPTY_ANIM_MAPPING;
        if (animMapping->length() > number && animMapping->at(number).length() > anim.number) {
            mappingForAnim = animMapping->at(number).at(anim.number);
        }

        QString filename;
        if (mappingForAnim.name == "") {
            filename = "s" + QString::number(number) + "_a" + QString::number(anim.number) + ".png";
        } else {
            if (!outDir.exists(mappingForAnim.category)) {
                outDir.mkdir(mappingForAnim.category);
            }
            filename = mappingForAnim.category + "/" + mappingForAnim.name + ".png";
        }

        anim.adjustSize();
        anim.writeImageFile(outDir, outDir.absoluteFilePath(filename), number, mappingForAnim.palette, progressOutput);
        anim.writeMetaJsonFile(outDir, outDir.absoluteFilePath(filename + ".json"), number, version, progressOutput);
    }

    for (auto& sample : audioSamples) {
        SampleMapping mappingForSample = IDMapper::EMPTY_SAMPLE_MAPPING;
        if (sampleMapping->length() > number && sampleMapping->at(number).length() > sample.number) {
            mappingForSample = sampleMapping->at(number).at(sample.number);
        }

        QString filename;
        if (mappingForSample.name == "") {
            filename = "s" + QString::number(number) + "_s" + QString::number(sample.number) + ".wav";
        } else {
            if (!outDir.exists(mappingForSample.category)) {
                outDir.mkdir(mappingForSample.category);
            }
            filename = mappingForSample.category + "/" + mappingForSample.name + ".wav";
        }


        sample.writeAsWavFile(outDir, outDir.absoluteFilePath(filename), number, progressOutput);
    }
}

const QVector<Jazz2Anim>* Jazz2AnimSet::getAnimations() const {
    return &animations;
}

const QVector<Jazz2Sample>* Jazz2AnimSet::getSamples() const {
    return &audioSamples;
}

void Jazz2AnimSet::readAnimations(quint8 animCount, Jazz2FormatDataBlock& metadata, Jazz2FormatDataBlock& frameData, const QByteArray* const imageData) {
    for (quint16 j = 0; j < animCount; ++j) {
        Jazz2Anim anim;
        anim.number = j;
        anim.frameCnt = metadata.readUShort();
        anim.fps = metadata.readUShort();
        anim.normalizedHotspot = { 0, 0 };
        anim.adjustedSize = { 0, 0 };

        // Skip the rest, seems to be 0x00000000 for all headers
        // Docs state this is used internally in the exe for... something
        metadata.discardBytes(4);

        if (anim.frameCnt > 0) {
            QPair<qint16, qint16> lastColdspot;
            QPair<qint16, qint16> lastGunspot;
            QPair<qint16, qint16> lastHotspot;

            for (quint16 j = 0; j < anim.frameCnt; j++) {
                Jazz2AnimFrame frame;
                quint16 x, y;

                x = frameData.readUShort();
                y = frameData.readUShort();
                frame.size = qMakePair(x, y);

                x = frameData.readUShort();
                y = frameData.readUShort();
                frame.coldspot = qMakePair(x, y);

                x = frameData.readUShort();
                y = frameData.readUShort();
                frame.hotspot = qMakePair(x, y);

                x = frameData.readUShort();
                y = frameData.readUShort();
                frame.gunspot = qMakePair(x, y);

                frame.imageAddr = frameData.readUInt();
                frame.maskAddr = frameData.readUInt();

                // Adjust normalized position
                // In the output images, we want to make the hotspot and image size constant.
                anim.normalizedHotspot.first = std::max(qint16(-frame.hotspot.first), anim.normalizedHotspot.first);
                anim.normalizedHotspot.second = std::max(qint16(-frame.hotspot.second), anim.normalizedHotspot.second);
                anim.largestOffset.first = std::max(qint16(frame.size.first + frame.hotspot.first), anim.largestOffset.first);
                anim.largestOffset.second = std::max(qint16(frame.size.second + frame.hotspot.second), anim.largestOffset.second);
                anim.adjustedSize.first = std::max(
                    qint16(anim.normalizedHotspot.first + anim.largestOffset.first),
                    anim.adjustedSize.first
                );
                anim.adjustedSize.second = std::max(
                    qint16(anim.normalizedHotspot.second + anim.largestOffset.second),
                    anim.adjustedSize.second
                );

                lastColdspot = frame.coldspot;
                lastGunspot = frame.gunspot;
                lastHotspot = frame.hotspot;

                quint32 dpos = frame.imageAddr + 4;

                quint16 width2 = ((quint16)(imageData->mid(dpos - 3, 1)[0] & 0xFF) << 8) + ((quint16)(imageData->mid(dpos - 4, 1)[0] & 0xFF));
                quint16 height2 = ((quint16)(imageData->mid(dpos - 1, 1)[0] & 0xFF) << 8) + ((quint16)(imageData->mid(dpos - 2, 1)[0] & 0xFF));
                frame.drawTransparent = (width2 & 0x8000) > 0;

                quint16 pxRead = 0;
                quint16 pxTotal = frame.size.first * frame.size.second;
                bool lastOpEmpty = true;

                while (pxRead < pxTotal) {
                    if (dpos > 0x10000000) {
                        //std::cerr << "WARNING: loading image data probably failed! Aborting.\n";
                        break;
                    }
                    quint8 op = imageData->mid(dpos, 1)[0];

                    if (op == 0) {
                        //std::cerr << "WARNING: next image operation should probably not be 0x00\n";
                    }

                    if (op < 0x80) {
                        // Skip the given number of pixels, writing them with the transparent color 0
                        pxRead += op;
                        while (op-- > 0) {
                            frame.imageData.append((char)0x00);
                        }
                        dpos++;
                    } else if (op == 0x80) {
                        // Skip until the end of the line
                        quint16 linePxLeft = frame.size.first - pxRead % frame.size.first;
                        if (pxRead % frame.size.first == 0 && !lastOpEmpty) {
                            linePxLeft = 0;
                        }

                        pxRead += linePxLeft;
                        while (linePxLeft-- > 0) {
                            frame.imageData.append((char)0x00);
                        }
                        dpos++;
                    } else {
                        // Copy specified amount of pixels (ignoring the high bit)
                        quint16 bytesToRead = op & 0x7F;
                        QByteArray nextData = imageData->mid(dpos + 1, bytesToRead);
                        frame.imageData.append(nextData);
                        pxRead += bytesToRead;
                        dpos += bytesToRead + 1;
                    }

                    lastOpEmpty = (op == 0x80);
                }

                frame.maskData.fill(false, pxTotal);
                dpos = frame.maskAddr;
                pxRead = 0;

                // no mask
                if (dpos != 0xFFFFFFFF) {
                    while (pxRead < pxTotal) {
                        quint8 byte = imageData->mid(dpos, 1)[0];
                        for (quint8 bit = 0; bit < 8 && (pxRead + bit) < pxTotal; ++bit) {
                            frame.maskData.setBit(pxRead + bit, byte & (1 << (7 - bit)));
                        }
                        pxRead += 8;
                    }
                }

                anim.frames.append(frame);
            }

            animations << anim;
        }
    }
}

void Jazz2AnimSet::readSamples(quint8 sampleCount, Jazz2FormatDataBlock& sampleData) {
    for (quint16 j = 0; j < sampleCount; ++j) {
        Jazz2Sample sample;
        sample.number = j;

        quint32 totalSize = sampleData.readUInt();
        quint32 magicRIFF = sampleData.readUInt();
        quint32 chunkSize = sampleData.readUInt();
        // "ASFF" for 1.20, "AS  " for 1.24
        quint32 format = sampleData.readUInt();
        bool isASFF = format == 0x46465341;

        quint32 magicSAMP = sampleData.readUInt();
        quint32 sampSize = sampleData.readUInt();
        // Padding/unknown data #1
        // For set 0 sample 0:
        //       1.20                           1.24
        //  +00  00 00 00 00 00 00 00 00   +00  40 00 00 00 00 00 00 00
        //  +08  00 00 00 00 00 00 00 00   +08  00 00 00 00 00 00 00 00
        //  +10  00 00 00 00 00 00 00 00   +10  00 00 00 00 00 00 00 00
        //  +18  00 00 00 00               +18  00 00 00 00 00 00 00 00
        //                                 +20  00 00 00 00 00 40 FF 7F
        sampleData.discardBytes(40 - (isASFF ? 12 : 0));
        if (isASFF) {
            // All 1.20 samples seem to be 8-bit. Some of them are among those
            // for which 1.24 reads as 24-bit but that might just be a mistake.
            sampleData.discardBytes(2);
            sample.multiplier = 0;
        } else {
            // for 1.24. 1.20 has "20 40" instead in s0s0 which makes no sense
            sample.multiplier = sampleData.readUShort();
        }

        // Unknown. s0s0 1.20: 00 80, 1.24: 80 00
        sampleData.discardBytes(2);

        quint32 payloadSize = sampleData.readUInt();

        // Padding #2, all zeroes in both
        sampleData.discardBytes(8);

        sample.sampleRate = sampleData.readUInt();
        quint32 actualDataSize = chunkSize - 76 + (isASFF ? 12 : 0);

        sample.soundData = sampleData.readRawBytes(actualDataSize);
        // Padding #3
        sampleData.discardBytes(4);

        if (magicRIFF != 0x46464952 || magicSAMP != 0x504D4153) {
            throw Jazz2FormatParseException(INVALID_MAGIC);
        }

        if (sample.soundData.length() < actualDataSize) {
            throw Jazz2FormatParseException(UNEXPECTED_FILE_LENGTH);
        }

        if (totalSize > chunkSize + 12) {
            // Sample data is probably aligned to X bytes since the next sample doesn't always appear
            // right after the first ends.
            sampleData.discardBytes(totalSize - chunkSize - 12);
        }

        audioSamples << sample;
    }
}
