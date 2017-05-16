#pragma once
#include <iostream>
#include <memory>

#include <QVector>
#include <QByteArray>
#include <QFile>
#include <QDir>

#include "Jazz2FormatDataBlock.h"
#include "IDMapping.h"
#include "Jazz2AnimLib.h"
#include "Jazz2Anim.h"
#include "Jazz2Sample.h"

class Jazz2AnimSet {
public:
    Jazz2AnimSet(quint16 number, Jazz2FormatDataBlock& data);
    Jazz2AnimSet(quint16 number, const QString& error);
    void writeAssetsToRawFiles(const QDir& outDir, std::shared_ptr<AnimIDMap> animMapping, std::shared_ptr<SampleIDMap> sampleMapping, Jazz2AnimVersion version, std::ostream& progressOutput);
    const QVector<Jazz2Anim>* getAnimations() const;
    const QVector<Jazz2Sample>* getSamples() const;

private:
    quint16 number;
    QVector<Jazz2Anim> animations;
    QVector<Jazz2Sample> audioSamples;

    bool placeholder;
    QString errorString;

    void readAnimations(quint8 animCount, Jazz2FormatDataBlock& metadata, Jazz2FormatDataBlock& frameData, const QByteArray* const imageData);
    void readSamples(quint8 sampleCount, Jazz2FormatDataBlock& sampleData);
};
