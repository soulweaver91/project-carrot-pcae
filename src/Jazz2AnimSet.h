#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <QVector>
#include <QPair>
#include <QByteArray>
#include <QBitArray>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "Jazz2FormatDataBlock.h"
#include "Jazz2FormatParseException.h"
#include "IDMapping.h"
#include "Version.h"
#include "Jazz2AnimLib.h"

struct Jazz2AnimFrame {
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

struct Jazz2Anim {
    quint16 number;
    quint16 frameCnt;
    quint16 fps;

    QVector<Jazz2AnimFrame> frames;

    QPair<qint16, qint16> adjustedSize;
    QPair<qint16, qint16> largestOffset;
    QPair<qint16, qint16> normalizedHotspot;
    QPair<qint16, qint16> frameConfiguration;

    void adjustSize() {
        // Determine the frame configuration to use.
        // Each asset must fit into a 4096 by 4096 texture,
        // as that is the smallest texture size we have decided to support.
        frameConfiguration.first = frameCnt;
        frameConfiguration.second = 1;
        if (frameCnt > 1) {
            frameConfiguration.second = std::max(1, (int)std::ceil(std::sqrt(frameCnt * adjustedSize.first / adjustedSize.second)));
            frameConfiguration.first = std::max(1, (int)std::ceil(frameCnt * 1.0 / frameConfiguration.second));

            // Do a bit of optimization, as the above algorithm ends occasionally with some extra space
            // (it is careful with not underestimating the required space)
            while (frameConfiguration.first * (frameConfiguration.second - 1) >= frameCnt) {
                frameConfiguration.second--;
            }
        }
    }

    void writeImageFile(const QString& filename, const sf::Color palette[256]) const {
        sf::Image img;
        img.create(adjustedSize.first * frameConfiguration.first,
                   adjustedSize.second * frameConfiguration.second, sf::Color(0, 0, 0, 0));

        for (int j = 0; j < frames.length(); ++j) {
            const Jazz2AnimFrame* frame = &frames.at(j);
            quint32 offsetX = normalizedHotspot.first + frame->hotspot.first;
            quint32 offsetY = normalizedHotspot.second + frame->hotspot.second;

            for (quint16 y = 0; y < frame->size.second; ++y) {
                for (quint16 x = 0; x < frame->size.first; ++x) {
                    quint16 targetX = (j % frameConfiguration.first) * adjustedSize.first + offsetX + x;
                    quint16 targetY = (j / frameConfiguration.first) * adjustedSize.second + offsetY + y;
                    quint8 colorIdx = frame->imageData.at(frame->size.first * y + x);
                    sf::Color color = palette[colorIdx];
                    if (frame->drawTransparent) {
                        color.a = std::min(sf::Uint8(127), color.a);
                    }

                    img.setPixel(targetX, targetY, color);
                }
            }
        }

        img.saveToFile(filename.toStdString());
    }
    void writeMetaJsonFile(const QString& filename, quint16 set, Jazz2AnimVersion version) const {
        QJsonObject meta;
        meta.insert("description", "Set " + QString::number(set) + " anim " + QString::number(number));
        meta.insert("generator", "Project Carrot Anim Extractor v" + QString::fromStdString(PCAE_VERSION));

        QString verString;
        switch (version) {
            case Jazz2AnimVersion::ORIGINAL: verString = "1.20/.23"; break;
            case Jazz2AnimVersion::TSF:      verString = "1.24"; break;
            default:                   verString = "unknown";
        }

        meta.insert("animsVersion", verString);

        QJsonObject root;
        root.insert("width", QJsonValue(adjustedSize.first));
        root.insert("height", QJsonValue(adjustedSize.second));
        root.insert("framesPerRow", QJsonValue(frameConfiguration.first));
        root.insert("framesPerCol", QJsonValue(frameConfiguration.second));
        root.insert("frameCount", QJsonValue(frameCnt));
        root.insert("hotspot", QJsonValue(QJsonArray({ normalizedHotspot.first, normalizedHotspot.second })));
        if (frames[0].coldspot.first == 0 && frames[0].coldspot.second == 0) {
            root.insert("coldspot", QJsonValue::Null);
        } else {
            root.insert("coldspot", QJsonValue(QJsonArray({
                (normalizedHotspot.first + frames[0].hotspot.first) - frames[0].coldspot.first,
                (normalizedHotspot.second + frames[0].hotspot.second) - frames[0].coldspot.second
            })));
        }

        if (frames[0].gunspot.first == 0 && frames[0].gunspot.second == 0) {
            root.insert("gunspot", QJsonValue::Null);
        } else {
            root.insert("gunspot", QJsonValue(QJsonArray({
                (normalizedHotspot.first + frames[0].hotspot.first) - frames[0].gunspot.first,
                (normalizedHotspot.second + frames[0].hotspot.second) - frames[0].gunspot.second
            })));
        }

        root.insert("fps", QJsonValue(fps));
        root.insert("scaling", QJsonValue(1));
        root.insert("meta", meta);

        QJsonDocument doc;
        doc.setObject(root);

        QFile jsonfh(filename);
        if (!(jsonfh.open(QIODevice::WriteOnly))) {
            throw Jazz2FormatParseException(FILE_CANNOT_BE_OPENED, { filename });
        }

        jsonfh.write(doc.toJson());
        jsonfh.close();
    }
};

struct Jazz2Sample {
    quint16 number;

    quint32 sampleRate;
    QByteArray soundData;
    quint16 multiplier;

    void writeAsWavFile(const QString& filename) const {
        QFile waveFile(filename);
        if (!(waveFile.open(QIODevice::ReadWrite))) {
            throw Jazz2FormatParseException(FILE_CANNOT_BE_OPENED, { filename });
        }
        // If overwriting, make sure nothing is left behind in case the new version is shorter.
        waveFile.resize(0);

        QDataStream stream(&waveFile);
        stream.setByteOrder(QDataStream::LittleEndian);

        // TODO: The modulo here essentially clips the sample to 8- or 16-bit.
        // There are some samples (at least the Rapier random noise) that at least get reported as 24-bit
        // by the read header data. It is not clear if they actually are or if the header data is just
        // read incorrectly, though - one would think the data would need to be reshaped between 24 and 8
        // but it works just fine as is.
        quint16 effectiveMultiplier = (multiplier / 4) % 2 + 1;

        // Main header
        stream.writeRawData("RIFF", 4);
        stream << (quint32)(soundData.length() + 36);
        stream.writeRawData("WAVE", 4);

        // Format header
        stream.writeRawData("fmt ", 4);
        // Write header remaining length (16 bytes), format (PCM) and channels (1)
        stream << (quint32)16 << (quint16)1 << (quint16)1;
        stream << sampleRate; // sample rate
        stream << (sampleRate * effectiveMultiplier); // byte rate
        stream << (quint32)(effectiveMultiplier * 0x00080001);

        // Payload
        stream.writeRawData("data", 4);
        stream << (quint32)soundData.length(); // payload length

        // Transform the data, then push it into the file.
        QByteArray transformedData(soundData);
        for (int i = 0; i < transformedData.length(); ++i) {
            transformedData[i] = (char)((effectiveMultiplier << 7) ^ transformedData.at(i));
        }
        stream.writeRawData(transformedData.data(), transformedData.size());

        waveFile.close();
    }
};

class Jazz2AnimSet {
public:
    Jazz2AnimSet(quint16 number, Jazz2FormatDataBlock& data);
    void writeAssetsToRawFiles(const QDir& outDir, std::shared_ptr<AnimIDMap> animMapping, std::shared_ptr<SampleIDMap> sampleMapping, Jazz2AnimVersion version);
    const QVector<Jazz2Anim>* getAnimations() const;
    const QVector<Jazz2Sample>* getSamples() const;

private:
    quint16 number;
    QVector<Jazz2Anim> animations;
    QVector<Jazz2Sample> audioSamples;

    void readAnimations(quint8 animCount, Jazz2FormatDataBlock& metadata, Jazz2FormatDataBlock& frameData, const QByteArray* const imageData);
    void readSamples(quint8 sampleCount, Jazz2FormatDataBlock& sampleData);
};
