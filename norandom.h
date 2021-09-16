#ifndef NO_RANDOM_H
#define NO_RANDOM_H

#include "command.h"
#include <vector>
#include <string>

class Player;

// Replaces the player's current block factory with a file block factory
// The name of the file to use should be provided as the first element
//  in the args vector for ::run
class NoRandom : public Command {
    public:

    // Expects args to have size == 1
    // The argument should be the name of the file to use for block generation
    void run(Player&, int count, std::vector<std::string> args) override;
};

#endif
