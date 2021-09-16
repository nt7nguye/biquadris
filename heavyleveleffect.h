#ifndef HEAVY_LEVEL_EFFECT_H
#define HEAVY_LEVEL_EFFECT_H

#include "statuseffect.h"

class Player;
class Command;

class HeavyLevelEffect : public StatusEffect {
    public:
    void afterCommand(std::unique_ptr<Command> const& cmd, Player& player) override;
    bool isForLevel() const override;
};

#endif
