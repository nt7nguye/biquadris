#ifndef STATUS_EFFECT_H
#define STATUS_EFFECT_H

#include <memory>

class Command;
class Player;

// A status effect updates a Player's state when specific hooks are called
// If a status effect is attached to a player, it will be called
// - immediately after the player's turn begins (before they have executed
//    any commands)
// - before that player executes a command
// - after that player executes a command
// - immediately before the player's turn ends (after they have executed
//    all commands for that turn)
class StatusEffect {
    public:
    virtual void beforeCommand(std::unique_ptr<Command> const&, Player&);
    virtual void afterCommand(std::unique_ptr<Command> const&, Player&);
    virtual void beforeTurn(Player&);
    virtual void afterTurn(Player&);
    virtual bool finished() const;
    virtual bool isForLevel() const;
    virtual ~StatusEffect() = default;
};

#endif
