#pragma once

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <QVector>
#include <QPair>
#include <QByteArray>
#include <QBitArray>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QString>

#include "Version.h"
#include "Jazz2FormatParseException.h"

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

    bool placeholder;
    QString errorString;

    Jazz2Anim() : placeholder(false) {

    }

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

    void writeImageFile(const QDir& outDir, const QString& filename, quint16 set, const sf::Color palette[256], std::ostream& progressOutput) const {
        if (placeholder) {
            progressOutput << " NOT EXTRACTED: Set " << QString::number(set).rightJustified(3).toStdString()
                << " anim   " << QString::number(number).rightJustified(3).toStdString()
                << ". Reading errored out due to: " << errorString.toStdString() << "\n";
            return;
        }

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
        QFileInfo writtenFileData(filename);
        progressOutput << " << " << outDir.relativeFilePath(filename).leftJustified(60).toStdString()
            << " Set " << QString::number(set).rightJustified(3).toStdString()
            << " anim   " << QString::number(number).rightJustified(3).toStdString()
            << ", " << QString::number(writtenFileData.size()).rightJustified(6).toStdString() << " bytes"
            << ", " << QString::number(img.getSize().x).rightJustified(3).toStdString()
            << "x" << QString::number(img.getSize().y).leftJustified(3).toStdString()
            << " PNG, " << QString::number(frameCnt).rightJustified(3).toStdString() << " frames of "
            << QString::number(adjustedSize.first).rightJustified(3).toStdString()
            << "x" << QString::number(adjustedSize.second).leftJustified(3).toStdString() << " @ "
            << QString::number(fps).rightJustified(2).toStdString() << "FPS\n";
    }
    void writeMetaJsonFile(const QDir& outDir, const QString& filename, quint16 set, Jazz2AnimVersion version, std::ostream& progressOutput) const {
        if (placeholder) {
            return;
        }

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

        QFileInfo writtenFileData(filename);
        progressOutput << " << " << outDir.relativeFilePath(filename).leftJustified(60).toStdString()
            << " Set " << QString::number(set).rightJustified(3).toStdString()
            << " anim   " << QString::number(number).rightJustified(3).toStdString()
            << ", " << QString::number(writtenFileData.size()).rightJustified(6).toStdString() << " bytes"
            << ",        JSON\n";
    }
};
