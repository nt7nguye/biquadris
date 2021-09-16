#ifndef MOVE_BLOCK_H
#define MOVE_BLOCK_H

#include "command.h"
#include <vector>
#include <string>

class Player;

// Moves the active block of the player in a direction
// The block will not be moved if doing so would cause a collision 
//  on the player's board
class MoveBlock : public Command {
    public:
    enum Direction { Left, Right, Down };

    private:
    Direction dir;

    public:
    MoveBlock(Direction dir);
    void run(Player&, int count, std::vector<std::string> args) override;
    Move moveType() const override;
};

#endif
