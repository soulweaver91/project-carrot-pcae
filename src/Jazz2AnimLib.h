#pragma once
#include <memory>
#include <iostream>

#include <QString>
#include <QVector>
#include <QByteArray>
#include <QDir>

#include "Jazz2FormatDataBlock.h"

enum Jazz2AnimVersion {
    ORIGINAL,
    TSF,
    CC,
    HH
};

class Jazz2AnimSet;

class Jazz2AnimLib {
public:
    Jazz2AnimLib();
    static Jazz2AnimLib* fromFile(const QString& filename, bool strictParser = false);
    void extractAllResources(const QDir& directory, std::ostream& progressOutput);

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
