#include "Jazz2AnimLib.h"
#include "Jazz2AnimSet.h"
#include "Jazz2FormatParseException.h"
#include "IDMapping.h"

Jazz2AnimLib::Jazz2AnimLib() {
}

Jazz2AnimLib* Jazz2AnimLib::fromFile(const QString& filename, bool strictParser) {
    QFile fh(filename);
    if (!(fh.exists())) {
        throw Jazz2FormatParseException(FILE_NOT_FOUND, { filename });
    }

    if (!(fh.open(QIODevice::ReadOnly))) {
        throw Jazz2FormatParseException(FILE_CANNOT_BE_OPENED, { filename });
    }

    Jazz2FormatDataBlock headerBlock(fh.read(28), false, 28);
    auto lib = new Jazz2AnimLib(headerBlock, fh, fh.size(), false);

    QByteArray setRawData = fh.readAll();

    // All data is in the memory, so the file can be closed.
    fh.close();

    lib->loadSets(setRawData);

    return lib;
}

void Jazz2AnimLib::extractAllResources(const QDir& directory, std::ostream& progressOutput) {
    progressOutput << "Detected as Anims.j2a for Jazz Jackrabbit 2";
    switch (version) {
        case Jazz2AnimVersion::ORIGINAL:
            progressOutput << " (1.20-1.23)";
            break;
        case Jazz2AnimVersion::HH:
            progressOutput << " Holiday Hare (1.23x)";
            break;
        case Jazz2AnimVersion::TSF:
            progressOutput << ": The Secret Files (1.24)";
            break;
        case Jazz2AnimVersion::CC:
            progressOutput << " Christmas Chronicles (1.24x)";
            break;
    }
    progressOutput << ".\n";

    uint i = 0;
    uint animCount = 0;
    uint sampleCount = 0;
    for (auto set : sets) {
        uint anims = set->getAnimations()->length();
        uint samples = set->getSamples()->length();
        progressOutput << " Set " << QString::number(i).rightJustified(3).toStdString() << ": "
            << QString::number(anims).rightJustified(4).toStdString() << " anims and "
            << QString::number(samples).rightJustified(4).toStdString() << " samples.\n";
        ++i;
        animCount += anims;
        sampleCount += samples;
    }
    progressOutput << "In total, "
        << QString::number(animCount).rightJustified(4).toStdString() << " anims and "
        << QString::number(sampleCount).rightJustified(4).toStdString() << " samples.\n\n";

    std::shared_ptr<AnimIDMap> animMapping = IDMapper::getAnimMap(version);
    std::shared_ptr<SampleIDMap> sampleMapping = IDMapper::getSampleMap(version);

    for (auto& set : sets) {
        set->writeAssetsToRawFiles(directory, animMapping, sampleMapping, version, progressOutput);
    }
}

Jazz2AnimLib::Jazz2AnimLib(Jazz2FormatDataBlock& header, QFile& fileHandle, quint32 fileLength, bool strictParser) {
    // Magic bytes ALIB and BABE
    qint32 magicALIB = header.readUInt();
    qint32 magicBABE = header.readUInt();
    if (magicALIB != 0x42494C41 || magicBABE != 0x00BEBA00) {
        throw Jazz2FormatParseException(INVALID_MAGIC);
    }

    // 1.20  JJ2 vanilla, 1.23 JJ2 patched: 464 bytes
    // 1.23x JJ2 HH: 476 bytes
    // 1.24  JJ2 TSF: 500 bytes
    headerLength = header.readUInt();

    qint32 magicUnknown = header.readUInt();
    if (magicUnknown != 0x18080200) {
        throw Jazz2FormatParseException(INVALID_MAGIC);
    }

    quint32 recordedSize = header.readUInt();
    if (strictParser && fileLength != recordedSize) {
        throw Jazz2FormatParseException(UNEXPECTED_FILE_LENGTH);
    }

    // Get the CRC; would check here if it matches if we knew what variant it is AND what it applies to
    // Test file across all CRC32 variants + Adler had no matches to the value obtained from the file
    // so either the variant is something else or the CRC is not applied to the whole file but on a part
    quint32 recordedCRC = header.readUInt();

    quint32 setCount = header.readUInt();
    sets.reserve(setCount);
    setAddresses.reserve(setCount);

    Jazz2FormatDataBlock setAddressBlock(fileHandle.read(4 * setCount), false, 4 * setCount);
    for (quint32 i = 0; i < setCount; ++i) {
        setAddresses << setAddressBlock.readUInt();
    }

    if (headerLength != fileHandle.pos()) {
        throw Jazz2FormatParseException(HEADER_SET_LENGTH_MISMATCH);
    }
}

void Jazz2AnimLib::loadSets(QByteArray& rawData) {
    for (int i = 0; i < setAddresses.size(); ++i) {
        try {
            int length = -1;
            if ((i + 1) < setAddresses.size()) {
                length = setAddresses.at(i + 1) - setAddresses.at(i);
            }

            QByteArray setData = rawData.mid(setAddresses.at(i) - headerLength, length);
            Jazz2FormatDataBlock dataBlock(setData, false, setData.length());
            sets << std::make_shared<Jazz2AnimSet>(i, dataBlock);
        } catch (Jazz2FormatParseException e) {
            throw;
            // TODO: warn about skipped set
            // TODO: actually insert an empty set
        }
    }

    bool seemsLikeCC = (sets.at(65)->getAnimations()->length() > 5);
    if (headerLength == 464) {
        version = Jazz2AnimVersion::ORIGINAL;
    } else if (headerLength == 500 && seemsLikeCC) {
        version = Jazz2AnimVersion::CC;
    } else if (headerLength == 500 && !seemsLikeCC) {
        version = Jazz2AnimVersion::TSF;
    } else if (headerLength == 476) {
        version = Jazz2AnimVersion::HH;
    } else {
        version = Jazz2AnimVersion::UNKNOWN;
    }
}
