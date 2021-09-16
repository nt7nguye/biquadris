#ifndef FORCE_ACTION_H
#define FORCE_ACTION_H

#include <memory>
#include <string>
#include <vector>

#include "specialaction.h"
#include "statuseffect.h"
#include "cell.h"

class Player;

// Changes the affected player's next block to a block of the 
//  original player's  choice
class ForceAction: public SpecialAction {
    public:
    void run(
            Player& originalPlayer, 
            Player& affectedPlayer, 
            std::vector<std::string> const& args) override; 
    std::string getDesc() const override;
};

#endif
