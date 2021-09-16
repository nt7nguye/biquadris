#ifndef ROTATE_BLOCK_H
#define ROTATE_BLOCK_H

#include "command.h"
#include <vector>
#include <string>

class Player;

// Rotates the player's activeBlock in either a clockwise or 
//  counterclockwise direction. Does not perform a rotation if doing
//  so would cause the block to collide with the player's board
class RotateBlock : public Command {
    public:
    enum Direction { Clockwise, CounterClockwise };

    private: 
    Direction dir;

    public: 
    RotateBlock(Direction dir);
    void run(Player&, int count, std::vector<std::string> args) override;
    Move moveType() const override;
};

#endif
