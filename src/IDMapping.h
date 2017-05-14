#pragma once
#include <SFML/Graphics.hpp>
#include <QMap>
#include <QPair>
#include <QString>

#include "Jazz2AnimLib.h"
#include "Palette.h"

typedef QPair<quint32, quint32> SetResourcePair;
typedef QPair<QString, QString> ResourceMappingPair;

struct AnimMapping {
    QString category;
    QString name;
    const sf::Color* palette;

    AnimMapping() {};
    AnimMapping(QString c, QString n, const sf::Color* p = SPRITE_PALETTE) {
        category = c;
        name = n;
        palette = p;
    }
};

struct SampleMapping {
    QString category;
    QString name;

    SampleMapping() {};
    SampleMapping(QString c, QString n) {
        category = c;
        name = n;
    }
};

typedef QVector<QVector<AnimMapping>> AnimIDMap;
typedef QVector<QVector<SampleMapping>> SampleIDMap;

class IDMapper {
public:
    static const AnimMapping EMPTY_ANIM_MAPPING;
    static const SampleMapping EMPTY_SAMPLE_MAPPING;

    static const AnimIDMap BASE_GAME_ANIM_MAP;
    static std::shared_ptr<AnimIDMap> getAnimMap(Jazz2AnimVersion version);

    static const SampleIDMap BASE_GAME_SAMPLE_MAP;
    static std::shared_ptr<SampleIDMap> getSampleMap(Jazz2AnimVersion version);
private:
    IDMapper() = delete;
};