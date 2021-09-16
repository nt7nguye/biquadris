#ifndef BLIND_ACTION_H
#define BLIND_ACTION_H

#include "specialaction.h"
#include <memory>
#include <vector>
#include <string>

class Player;

// Hides a portion of the affected player's board
class BlindAction: public SpecialAction {
    public:
    void run(
            Player& originalPlayer, 
            Player& affectedPlayer, 
            std::vector<std::string> const& args) override;
    std::string getDesc() const override;
};


#endif
