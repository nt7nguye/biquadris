#ifndef HEAVY_EFFECT_H
#define HEAVY_EFFECT_H

#include "statuseffect.h"

class Player;
class Command;

// Causes the active block to drop 2 spaces after horizontal movement 
//  commands are run
class HeavyEffect : public StatusEffect {
    bool isFinished = false;

    public:
    void afterCommand(std::unique_ptr<Command> const&, Player&) override;
    void afterTurn(Player&) override;
    bool finished() const override;
};

#endif
