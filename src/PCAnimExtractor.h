#pragma once
#include <QString>
#include <QDir>

class PCAnimExtractor {
public:
    static void convert(const QString& filename, const QDir& outputDir);
};
