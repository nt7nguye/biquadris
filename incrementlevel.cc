#include "incrementlevel.h"
#include "levelfactory.h"
#include "player.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

IncrementLevel::IncrementLevel(Direction dir)
    : dir{dir}
{}

void IncrementLevel::run(Player& player, int count, vector<string>) {
    int newlevel = player.level;
    
    if (dir == Direction::Up) newlevel += count;
    else newlevel -= count;

    // Clamp level to valid range
    if (newlevel < LevelFactory::MinLevel) newlevel = LevelFactory::MinLevel;
    if (newlevel > LevelFactory::MaxLevel) newlevel = LevelFactory::MaxLevel;

    // Do nothing if the level would not change value
    if (newlevel == player.level) return;

    // Delete old level-based status effects
    StatusEffectArr& vec = player.statusEffects;
    vec.erase(remove_if(vec.begin(), vec.end(), 
              [](auto const& effect){ return effect->isForLevel(); }),
              vec.end());

    // Update the level
    player.level = newlevel;
    player.blockFactory = player.levelFactory->getBlockFactory(newlevel);
    player.board.setLevel(newlevel);

    // Player intitalization can use getStatusEffects to create an array
    // Increment Level should modify or it overrides other statuseffects
    StatusEffectArr arr = player.levelFactory->getStatusEffects(newlevel);
    for (auto& effect_pointer : arr) {
        player.statusEffects.emplace_back(move(effect_pointer));
    }
}
