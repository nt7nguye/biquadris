#include "garbagetower.h"
#include "player.h"
#include "command.h"
#include "block.h"
#include "cell.h"
#include "board.h"

unsigned const GarbageTower::GrowthRate = 5;

void GarbageTower::beforeTurn(Player& player) {
    rowsCleared = player.board.getTotalRowsCleared();
}

// If 5 turns have passed without a row being cleared then drop a 1x1 block
//  onto the game board in the center column
void GarbageTower::afterTurn(Player& player) {
    int newRowsCleared = player.board.getTotalRowsCleared();

    if (rowsCleared == newRowsCleared) {
        // No rows were cleared
        ++turnsWithoutClear;
    }
    else {
        turnsWithoutClear = 0;
    }

    if (turnsWithoutClear != 0 
        && turnsWithoutClear % GarbageTower::GrowthRate == 0) 
    {
        // It is time for the garbage tower to grow...
        // Drop a block onto the center of the board
        Block block = player.blockFactory->getBlock(
                CellType::Garbage, player.level, 
                Board::Width/2, Board::Height - 1); 
        Board& board = player.board;
        while (!board.causesCollision(block)) {
            block.moveDown();
        }
        block.moveUp();
        board.addBlock(block);
    }
}

bool GarbageTower::isForLevel() const {
    return true;
}

