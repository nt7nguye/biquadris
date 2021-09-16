#include "transformblock.h"
#include "blockfactory.h"
#include "player.h"
#include <vector>
#include <string>

using namespace std;

TransformBlock::TransformBlock(CellType type)
    : type{type}
{}

void TransformBlock::run(Player& player, int count, vector<string>) {
    // Create a new block in current position of the active block.
    // If there is no collision then replace the current active block.
    Block& block = player.activeBlock;
    Board& board = player.board;

    int oldx = block.getXPos();
    int oldy = block.getYPos();
    
    // Set to a new block
    Block temp =  player.blockFactory->getBlock(type, player.level, 
                                                        oldx, oldy);
    
    if (! board.causesCollision(temp)) {
        player.activeBlock = temp;
    }
}

Move TransformBlock::moveType() const {
    return Move::NoneMove;
}

