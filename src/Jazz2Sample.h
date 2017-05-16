#pragma once

#include <iostream>

#include <QByteArray>
#include <QDir>
#include <QString>
#include <QFile>
#include <QDataStream>

#include "Jazz2FormatParseException.h"

struct Jazz2Sample {
    quint16 number;

    quint32 sampleRate;
    QByteArray soundData;
    quint16 multiplier;

    bool placeholder;
    QString errorString;

    Jazz2Sample() : placeholder(false) {

    }

    void writeAsWavFile(const QDir& outDir, const QString& filename, quint16 set, std::ostream& progressOutput) const {
        if (placeholder) {
            progressOutput << " NOT EXTRACTED: Set " << QString::number(set).rightJustified(3).toStdString()
                << " sample " << QString::number(number).rightJustified(3).toStdString()
                << ". Reading errored out due to: " << errorString.toStdString() << "\n";
            return;
        }

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
        progressOutput << " << " << outDir.relativeFilePath(filename).leftJustified(60).toStdString()
            << " Set " << QString::number(set).rightJustified(3).toStdString()
            << " sample " << QString::number(number).rightJustified(3).toStdString()
            << ", " << QString::number(soundData.length() + 44).rightJustified(6).toStdString() << " bytes"
            << ",  " << QString::number(effectiveMultiplier * 8).rightJustified(2).toStdString() << "-bit WAV @ "
            << QString::number(sampleRate).rightJustified(5).toStdString() << "Hz\n";
    }
};