#pragma once
#include <QString>
#include <QVector>
#include <QPair>
#include <QByteArray>
#include <QBitArray>
#include <QDir>
#include "Jazz2FormatDataBlock.h"

enum Jazz2AnimVersion {
    ORIGINAL = 0x00000001,
    TSF      = 0x00000002,
    CC       = 0x00000004,
    HH       = 0x00000008,
    ALL      = 0xFFFFFFFF,
    UNKNOWN  = 0x00000000
};

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
    quint16 set;
    quint16 anim;

    quint16 frameCnt;
    quint16 fps;

    QVector<Jazz2AnimFrame> frames;

    QPair<qint16, qint16> adjustedSize;
    QPair<qint16, qint16> largestOffset;
    QPair<qint16, qint16> normalizedHotspot;
    QPair<qint16, qint16> frameConfiguration;
};

struct Jazz2Sample {
    quint16 set;
    quint16 idInSet;

    quint32 sampleRate;
    QByteArray soundData;
    quint16 multiplier;
};

class Jazz2AnimLib {
public:
    Jazz2AnimLib();
    static Jazz2AnimLib* fromFile(const QString& filename, bool strictParser = false);
    void extractAllResources(const QDir& directory);

private:
    QVector<Jazz2Anim> animations;
    QVector<Jazz2Sample> audioSamples;

    Jazz2AnimVersion version;

    // Parser utility functions
    Jazz2AnimLib(Jazz2FormatDataBlock& header, quint32 fileLength, bool strictParser);

    // PC file writing functions
};
