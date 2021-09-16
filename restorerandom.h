#ifndef RESTORE_RANDOM_H
#define RESTORE_RANDOM_H

#include "command.h"
#include <vector>
#include <string>

class Player;

// Sets the player's block generator to be the level's block generator
class RestoreRandom : public Command {
    public:
    void run(Player&, int count, std::vector<std::string> args) override;
};

#endif
