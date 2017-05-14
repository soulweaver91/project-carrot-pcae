#pragma once
#include <memory>

#include <QString>
#include <QVector>
#include <QByteArray>
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

class Jazz2AnimSet;

class Jazz2AnimLib {
public:
    Jazz2AnimLib();
    static Jazz2AnimLib* fromFile(const QString& filename, bool strictParser = false);
    void extractAllResources(const QDir& directory);

private:
    QVector<quint32> setAddresses;
    QVector<std::shared_ptr<Jazz2AnimSet>> sets;

    Jazz2AnimVersion version;
    quint32 headerLength;

    // Parser utility functions
    Jazz2AnimLib(Jazz2FormatDataBlock& header, QFile& fileHandle, quint32 fileLength, bool strictParser);
    void loadSets(QByteArray& rawData);

    // PC file writing functions
};
