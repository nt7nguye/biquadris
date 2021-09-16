#include "blindeffect.h"
#include "player.h"

using namespace std;

// Effects last for one turn only
void BlindEffect::afterTurn(Player& player) {
    player.isBlind = false;
    isFinished = true;
}

bool BlindEffect::finished() const {
    return isFinished;
}
