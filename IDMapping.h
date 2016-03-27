#pragma once
#include <SFML/Graphics.hpp>
#include <QMap>
#include <QPair>
#include <QString>
#include <version.h>

typedef QPair<quint32, quint32> SetResourcePair;
typedef QPair<QString, QString> ResourceMappingPair;

struct AnimMapping {
    QString category;
    QString name;
    const sf::Color* palette;

    AnimMapping(QString c, QString n, const sf::Color* p) {
        category = c;
        name = n;
        palette = p;
    }
};

struct SampleMapping {
    QString category;
    QString name;

    SampleMapping(QString c, QString n) {
        category = c;
        name = n;
    }
};

typedef QMap<SetResourcePair, AnimMapping> AnimIDMap;
typedef QMap<SetResourcePair, SampleMapping> SampleIDMap;

SampleMapping makeSampleMapping(std::string first, std::string second);
AnimMapping makeAnimMapping(std::string first, std::string second, const sf::Color* palette = SPRITE_PALETTE);
std::unique_ptr<AnimIDMap> getAnimMappingForVersion(JJ2Version version);
std::unique_ptr<SampleIDMap> getSampleMappingForVersion(JJ2Version version);
