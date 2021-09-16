#ifndef INCREMENT_LEVEL_H
#define INCREMENT_LEVEL_H

#include <vector>
#include <string>
#include "command.h"

class Player;

// Increases or decreases the level of a player some number of times 
// Ensures that the new level is clamped within a valid range, and 
//  updates the Player state such that it reflects the effects of the
//  new level.
// This includes:
//  - setting the block factory to be appropriate for the level
//  - adding any status effects to the player which are specific to the 
//     new level
//  - remove status effects from the old level
class IncrementLevel : public Command {
    public:
    enum Direction { Up, Down };

    private:
    Direction dir;

    public:
    IncrementLevel(Direction);
    void run(Player&, int count, std::vector<std::string> args) override;
};

#endif
