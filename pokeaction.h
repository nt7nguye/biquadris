#ifndef GARBAGE_ACTION_H
#define GARBAGE_ACTION_H

#include "specialaction.h"
#include <memory>
#include <string>
#include <vector>
#include "statuseffect.h"

class Player;

// Removes up to three cells from an opponents board
class PokeAction : public SpecialAction {
    public:
    void run(
            Player& originalPlayer, 
            Player& affectedPlayer, 
            std::vector<std::string> const& args) override;
    std::string getDesc() const override;
};

#endif
