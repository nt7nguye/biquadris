#include "moveblock.h"
#include "player.h"
#include <vector>
#include <string>

using namespace std;

MoveBlock::MoveBlock(Direction dir)
    : dir{dir}
{}

void MoveBlock::run(Player& player, int spacesToMove, vector<string>) {
    Block& block = player.activeBlock;
    Board& board = player.board;

    // Move the active block the requested number of times
    for (int i = 0; i < spacesToMove; ++i) {
        int oldx = block.getXPos();
        int oldy = block.getYPos();
        switch(dir) {
            case Direction::Left:
                block.moveLeft();
                break;
            case Direction::Right:
                block.moveRight();
                break;
            case Direction::Down:
                block.moveDown();
                break;
        }
        
        if (board.causesCollision(block)) {
            // Move caused a collision: undo the move
            block.setXPos(oldx);
            block.setYPos(oldy);
            break;
        }
    }
}

Move MoveBlock::moveType() const {
    switch(dir) {
        case Direction::Left:
            return Move::Left;
        case Direction::Right:
            return Move::Right;
        default:
            return Move::Down;
    }
}
