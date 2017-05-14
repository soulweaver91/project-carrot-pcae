#pragma once

#include <QVariant>
#include <QVector>

enum Jazz2FormatParseExceptionType {
    FILE_NOT_FOUND,
    FILE_CANNOT_BE_OPENED,
    INVALID_MAGIC,
    UNEXPECTED_FILE_LENGTH,
    BLOCK_DECOMPRESSION_FAILED,
    ASSET_FOLDER_CREATION_FAILED,
    HEADER_SET_LENGTH_MISMATCH
};

class Jazz2FormatParseException {
public:
    Jazz2FormatParseException(Jazz2FormatParseExceptionType type, QVector<QVariant> info = {});
    Jazz2FormatParseExceptionType getType();
    const QVector<QVariant> getInfo();
    QString friendlyText();

private:
    Jazz2FormatParseExceptionType type;
    QVector<QVariant> info;
};