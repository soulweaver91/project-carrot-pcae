#pragma once
#include <QString>
#include <QDir>

class PCAnimExtractor {
public:
    static void extract(const QString& filename, const QDir& outputDir);
};
