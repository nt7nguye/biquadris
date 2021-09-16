#ifndef TRANSFORM_BLOCK_H
#define TRANSFORM_BLOCK_H

#include "cell.h"
#include "command.h"
#include <vector>
#include <string>


class Player;

// Replaces the player's active block with a new type of active block
// The new block is in the same position as the old block
// The new block will not be replaced if doing so would cause a collision
//  on the player's board.
class TransformBlock : public Command {
    CellType type;

    public:
    TransformBlock(CellType type);
    void run(Player&, int count, std::vector<std::string> args) override;
    Move moveType() const override;
};

#endif
