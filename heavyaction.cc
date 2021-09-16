#include "heavyaction.h"
#include <memory>
#include <vector>
#include <string>
#include "statuseffect.h"
#include "heavyeffect.h"
#include "player.h"

using namespace std;

void HeavyAction::run(
        Player& originalPlayer, 
        Player& affectedPlayer, 
        vector<string> const& args) 
{
    // Get StatusEffect
    unique_ptr<StatusEffect> effect = 
        unique_ptr<StatusEffect>{ new HeavyEffect{} };

    // Place it on affected player
    affectedPlayer.statusEffects.emplace_back(move(effect));
}

string HeavyAction::getDesc() const {
    return "Next player's blocks fall twice after horizontal move";
}
