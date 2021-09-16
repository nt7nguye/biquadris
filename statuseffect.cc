#include "statuseffect.h"
#include <memory>
#include "player.h"
#include "command.h"

// Note: by default, a status effect does not do anything (hence the 
//  empty function implementations)

void StatusEffect::beforeCommand(std::unique_ptr<Command> const&, Player&){}
void StatusEffect::afterCommand(std::unique_ptr<Command> const&, Player&){}
void StatusEffect::afterTurn(Player&){}
void StatusEffect::beforeTurn(Player&){}
bool StatusEffect::finished() const{ return false; }
bool StatusEffect::isForLevel() const { return false; }
