#ifndef BLIND_EFFECT_H
#define BLIND_EFFECT_H

#include "statuseffect.h"
#include <memory>

class Player;
class Command;

// Hides a portion of the player's board.
// Lasts one turn
class BlindEffect : public StatusEffect {
    bool isFinished = false;

    public:
    void afterTurn(Player& player) override;
    bool finished() const override;
};

#endif
    
