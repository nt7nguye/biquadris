#include "rotateblock.h"
#include "board.h"
#include "block.h"
#include "player.h"
#include <vector>
#include <string>

using namespace std;

RotateBlock::RotateBlock(Direction dir)
    : dir{dir} 
{}

void RotateBlock::run(Player& player, int numRotations, vector<string>) {
    Block& block = player.activeBlock;
    Board& board = player.board;

    // Note that we only need to rotate %4 times,
    //  because 4 rotations is returns block to original position
    for (int i = 0; i < numRotations % 4; ++i) {
        switch(dir) {
            case Clockwise:
                block.rotateClockwise();
                break;
            case CounterClockwise:
                block.rotateCounterClockwise();
                break;
        }

        // If new position causes colision: undo
        if (board.causesCollision(block)) {
            switch(dir) {
                case Clockwise:
                    block.rotateCounterClockwise();
                    break;
                case CounterClockwise:
                    block.rotateClockwise();
                    break;
            }
        }
    }
}

Move RotateBlock::moveType() const {
    return Move::Rotate;
}
