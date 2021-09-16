#include "heavyleveleffect.h"
#include "player.h"
#include "command.h"
#include <memory>

using namespace std;

// Drops block 1 space after move or rotation commands
// Player loses if movement not possible
void HeavyLevelEffect::afterCommand(
        unique_ptr<Command> const& cmd, 
        Player& player) 
{
    if (cmd->moveType() == Move::NoneMove || cmd->moveType() == Move::Drop) 
        return; //command was not move or rotation type

    Block& block = player.activeBlock;
    Board& board = player.board;

    block.moveDown();
    
    // if block collides then don't move and end turn
    if (board.causesCollision(block)) {
        player.turnIsDone = true;
        block.moveUp();
    }
}

// Effect lasts for the level
bool HeavyLevelEffect::isForLevel() const {
    return true;
}
