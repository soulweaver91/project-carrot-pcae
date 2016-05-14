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

class IDMapper {
public:
    IDMapper(const JJ2Version& version);
    std::unique_ptr<AnimIDMap> getAnimMapping();
    std::unique_ptr<SampleIDMap> getSampleMapping();

    static const AnimMapping EMPTY_ANIM_MAPPING;
    static const SampleMapping EMPTY_SAMPLE_MAPPING;

private:
    static SampleMapping makeSampleMapping(std::string first, std::string second);
    static AnimMapping makeAnimMapping(std::string first, std::string second, const sf::Color* palette = SPRITE_PALETTE);
    const JJ2Version jj2Version;

    // Internal state, used in the sample mapping maker functions
    int currentSet;
    int currentItem;
};