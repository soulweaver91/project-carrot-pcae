#include "PCAnimExtractor.h"

#include <iostream>
#include <memory>
#include <QtCore/QCoreApplication>
#include <QString>
#include <QDataStream>
#include <QFile>
#include <QPair>
#include <QByteArray>
#include <QBitArray>
#include <QVector>
#include <QDir>
#include "Palette.h"
#include "IDMapping.h"
#include "Version.h"
#include "Jazz2AnimLib.h"
#include "Jazz2FormatParseException.h"

void PCAnimExtractor::extract(const QString& filename, const QDir& outputDir) {
    Jazz2AnimLib* anim = Jazz2AnimLib::fromFile(filename);
    //level->printData(std::cout);
    anim->extractAllResources(outputDir);
    //printDetailedEventStats(stats);
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 2) {
        std::cout << "PCAE version " << PCAE_VERSION << "\n"
            << "Usage: pcae path\n\n"
            << "Extracts resources from inside an Anims.j2a file to be used with Project Carrot.\n\n"
            << "Parameters:\n"
            << "  path         Required. Specifies where the Anims.j2a file is located.\n";
        return EXIT_FAILURE;
    }

    QString filename = argv[1];

    try {
        QDir outdir(QDir::current());
        if (!outdir.exists("Assets")) {
            if (!outdir.mkdir("Assets")) {
                throw Jazz2FormatParseException(ASSET_FOLDER_CREATION_FAILED);
            }
        }
        outdir.cd("Assets");

        std::cout << "Extracting resources from the selected animation library...\n";
        PCAnimExtractor::extract(filename, outdir);
        std::cout << "\nAssets have been extracted successfully to the Assets folder.\n"
            << "Combine this folder with the one in your Project Carrot installation folder\n"
            << "to use them.\n"
            << "Press Enter to continue...\n";
    } catch (Jazz2FormatParseException e) {
        std::cout << "ERROR: " << e.friendlyText().toStdString() << "\n";
        getchar();
        return EXIT_FAILURE;
    } catch (...) {
        std::cout << "Unknown error. Aborting conversion...\n";
        getchar();
        return EXIT_FAILURE;
    }

    getchar();
    return EXIT_SUCCESS;
}