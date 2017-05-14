#include "Jazz2FormatParseException.h"

Jazz2FormatParseException::Jazz2FormatParseException(Jazz2FormatParseExceptionType type, QVector<QVariant> info)
    : type(type), info(info) {

}

Jazz2FormatParseExceptionType Jazz2FormatParseException::getType() {
    return type;
}

const QVector<QVariant> Jazz2FormatParseException::getInfo() {
    return info;
}

QString Jazz2FormatParseException::friendlyText() {
    switch (type) {
        case FILE_NOT_FOUND:
            return "File " + getInfo().at(0).toString() + " was not found!";
            break;
        case FILE_CANNOT_BE_OPENED:
            return "File " + getInfo().at(0).toString() + " could not be opened!";
            break;
        case INVALID_MAGIC:
            return "File header not recognized as a .J2A file!";
            break;
        case UNEXPECTED_FILE_LENGTH:
            return "Conflicting file length reported inside the file!";
            break;
        case BLOCK_DECOMPRESSION_FAILED:
            return "Data block " + getInfo().at(0).toString() + " could not be uncompressed!";
            break;
        case ASSET_FOLDER_CREATION_FAILED:
            return "Could not create the asset folder!";
            break;
        case HEADER_SET_LENGTH_MISMATCH:
            return "Header length did not match with the number of sets in the library!";
            break;
        default:
            return "Unknown error!";
    }
}
