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
    HEADER_SET_LENGTH_MISMATCH,
    INVALID_SET_MAGIC,
    INVALID_SAMPLE_MAGIC,
    UNEXPECTED_SAMPLE_LENGTH,
    END_OF_IMAGE_DATA_NOT_FOUND
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