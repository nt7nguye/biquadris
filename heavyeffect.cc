#include "heavyeffect.h"
#include "player.h"
#include <iostream>
#include "command.h"
#include <memory>

using namespace std;

// Drops block 2 spaces on horizontal move
// Player loses if this movement is not possible
void HeavyEffect::afterCommand(
        unique_ptr<Command> const& cmd, 
        Player& player) 
{
    if (cmd->moveType() != Move::Left && cmd->moveType() != Move::Right) 
        return; //command was not a horizontal movement type

    Block& block = player.activeBlock;
    Board& board = player.board;

    for (int i = 0; i < 2; ++i) {
        block.moveDown();

        // If the block collides then the turn is done
        if (board.causesCollision(block)) {
            player.turnIsDone = true;
            block.moveUp();
            break;
        }
    }
}

// Effect lasts for 1 turn
void HeavyEffect::afterTurn(Player&) {
    isFinished = true;
}

bool HeavyEffect::finished() const {
    return isFinished;
}
