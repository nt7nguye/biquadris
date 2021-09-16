#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>

class Player;

enum class Move { NoneMove, Left, Right, Up, Down, Rotate, Drop };

// A Command is something which acts on a Player and updates 
//  its internal state. 
// In addition to a Player, each command is passed a "count" and "args"
//  on ::run().
class Command {
    public:
    virtual void run(Player&, int count, std::vector<std::string> args) = 0;
    virtual Move moveType() const;
    virtual ~Command() = default;
};

#endif
