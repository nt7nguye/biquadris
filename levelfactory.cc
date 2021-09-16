#include "levelfactory.h"
#include "blockfactory.h"
#include "fileblockfactory.h"
#include "randomblockfactory.h"
#include "statuseffect.h"
#include <vector>
#include <string>
#include <memory>
#include "garbagetower.h"
#include "heavyleveleffect.h"

using namespace std;

int const LevelFactory::MinLevel = 0;
int const LevelFactory::MaxLevel = 4;

LevelFactory::LevelFactory(string defaultFile) : defaultFile{defaultFile}
{}

BlockFactoryPtr LevelFactory::getBlockFactory(int level) const {
    if (level == 0) {
        return BlockFactoryPtr{ new FileBlockFactory{level, defaultFile} };
    } 
    else if (level == 1) {
        Distribution dist{2, 2, 2, 2, 1, 2, 1};
        return BlockFactoryPtr{ new RandomBlockFactory{level, dist} };
    }
    else if (level == 2) {
        Distribution dist{1, 1, 1, 1, 1, 1, 1};
        return BlockFactoryPtr{ new RandomBlockFactory{level, dist} };
    }
    else if (level == 3 || level == 4) {
        Distribution dist{1, 1, 1, 1, 2, 1, 2};
        return BlockFactoryPtr{ new RandomBlockFactory{level, dist} };
    }
    throw InvalidLevel{};
}

StatusEffectArr LevelFactory::getStatusEffects(int level) const {
    StatusEffectArr arr;
    if (level == 0 || level == 1 || level == 2) {
        return arr;
    }
    else if (level == 3) {
        arr.emplace_back(unique_ptr<StatusEffect>{ new HeavyLevelEffect });
    }
    else if (level == 4) {
        arr.emplace_back(unique_ptr<StatusEffect>{ new GarbageTower });
        arr.emplace_back(unique_ptr<StatusEffect>{ new HeavyLevelEffect });
    } else {
        throw InvalidLevel{};
    }
    return arr;
}
