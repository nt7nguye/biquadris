#ifndef LEVEL_FACTORY_H
#define LEVEL_FACTORY_H

#include "blockfactory.h"
#include "statuseffect.h"
#include <vector>
#include <string>
#include <memory>

typedef std::unique_ptr<BlockFactory> BlockFactoryPtr;
typedef std::vector<std::unique_ptr<StatusEffect>> StatusEffectArr;

class InvalidLevel{};

// Responsible for building the objects that are needed for a level
// Specifically a level is a BlockFactory plus zero of more status effects
// Calling the getters on this class returns the appropriate
//  objects for the requested level.
class LevelFactory {
    std::string defaultFile;

    public:
    static int const MinLevel;
    static int const MaxLevel;

    LevelFactory(std::string defaultFile);
    BlockFactoryPtr getBlockFactory(int level) const;
    StatusEffectArr getStatusEffects(int level) const;
};

#endif
