#ifndef SPECIAL_ACTION_H
#define SPECIAL_ACTION_H

#include <memory>
#include <string>
#include <vector>
#include "statuseffect.h"

class Player;

// Allows one player to "act" on another player.
// For example, the "heavy" action causes the affected player to have 
//  the "heavy" status effect be active on their next turn
class SpecialAction {
    public:
    virtual void run(
            Player& originalPlayer, 
            Player& affectedPlayer, 
            std::vector<std::string> const& args) = 0;
    virtual std::string getDesc() const = 0;
    virtual ~SpecialAction() = default;
};

#endif
