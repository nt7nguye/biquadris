#ifndef HEAVY_ACTION_H
#define HEAVY_ACTION_H

#include "specialaction.h"
#include <memory>
#include <string>
#include <vector>
#include "statuseffect.h"

class Player;

// Causes the affected player's blocks to drop 2 spaces per horizontal 
//  move on their next turn.
class HeavyAction : public SpecialAction {
    public:
    void run(
            Player& originalPlayer, 
            Player& affectedPlayer, 
            std::vector<std::string> const& args) override;
    std::string getDesc() const override;
};

#endif
