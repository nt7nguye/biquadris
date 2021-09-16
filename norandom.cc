#include "norandom.h"
#include "gameexceptions.h"
#include <vector>
#include <string>
#include "player.h"
#include "blockfactory.h"
#include "fileblockfactory.h"
#include <iostream>

using namespace std;

// Gives the player a file block factory
// The filename is the first argument
// Throws InvalidCommand if no arguments are provided
void NoRandom::run(Player& player, int, std::vector<std::string> args) {
    if (args.size() < 1) {
        // No file was provided
        throw InvalidCommand{};
    }

    string file = args[0];
    
    // Creation of a file block factory might fail if the requested file 
    //  does not exist or is otherwise invalid. 
    try {
        player.blockFactory = 
            unique_ptr<BlockFactory>{ new FileBlockFactory{ 
                player.level, 
                file 
            } };
    }
    catch(InvalidFile) {
        // Rethrow as an invalid command because that is the generic
        //  error for when a command fails
        throw InvalidCommand{};
    }
}
