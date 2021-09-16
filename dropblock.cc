#include "dropblock.h"
#include "player.h"
#include "block.h"
#include "board.h"
#include <vector>
#include <string>

using namespace std;

// Move block as far down the board as possible
// Then, add block to the board and end the player's turn
void DropBlock::run(Player& player, int timesToDrop, vector<string>) {
    for (int i = 0; i < timesToDrop; ++i) {
        Block& block = player.activeBlock;
        Board& board = player.board;

        // Move block down until collision occurs
        block.moveDown();
        while (!board.causesCollision(block)) {
            block.moveDown();
        }

        // Return block to previous non-colliding position
        block.moveUp();

        if (i < timesToDrop - 1) {
            // There are more blocks to drop
            // Add block to board and get the next block 
            board.addBlock(block);
            player.activeBlock = move(player.nextBlock);
            player.nextBlock = player.blockFactory->next();

            // If the new block collides then the turn is done
            if (board.causesCollision(player.activeBlock)) break;
        }
    }
    
    if (timesToDrop > 0) {
        player.turnIsDone = true;
    }
}

Move DropBlock::moveType() const {
    return Move::Drop;
}
