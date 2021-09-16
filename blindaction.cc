#include "blindaction.h"
#include "blindeffect.h"
#include "player.h"

#include <vector>
#include <string>

using namespace std;

void BlindAction::run(
        Player& originalPlayer, 
        Player& affectedPlayer, 
        vector<string> const& args) 
{
    // BlindAction set blind attribute directly, 
    //  but also keep a statuseffect to make sure it is reset after turn ends 
    
    affectedPlayer.isBlind = true;
    // Blind Action place Blind Effect on affected Player
    unique_ptr<StatusEffect> effect = 
        unique_ptr<StatusEffect>{ new BlindEffect{} };
    affectedPlayer.statusEffects.emplace_back(move(effect));
}

string BlindAction::getDesc() const {
    return "Blind the other player from column 3-9, row 3-12";
}
