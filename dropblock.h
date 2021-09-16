#ifndef DROP_BLOCK_H
#define DROP_BLOCK

#include "command.h"
#include <vector>
#include <string>

class Player;

// Causes the active block to fall its maximum distance and be added to 
//  the board.
// If a count is provided, then that many blocks are dropped in sequence
//  for the SAME player. Blocks will stop being dropped if at any point
//  the previously dropped block caused the player to lose.
class DropBlock : public Command {
    public:
    void run(Player&, int count, std::vector<std::string> args) override;
    Move moveType() const override;
};

#endif
