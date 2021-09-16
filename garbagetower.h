#ifndef GARBAGE_TOWER_H
#define GARBAGE_TOWER_H

#include "statuseffect.h"

class Player;
class Command;

// Every time 5 blocks are placed without clearing a row, a 1x1 block
//  is dropped onto the game board in the center column
class GarbageTower : public StatusEffect {
    static unsigned const GrowthRate;

    bool isFinished = false;
    int turnsWithoutClear = 0;
    int rowsCleared = 0;

    public:
    void beforeTurn(Player&) override;
    void afterTurn(Player&) override;
    bool isForLevel() const override;
};

#endif
