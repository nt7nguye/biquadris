#include "player.h"
#include <memory>
#include <vector>
#include "levelfactory.h"
#include "blockfactory.h"

using namespace std;

Player::Player(int id, int level, unique_ptr<LevelFactory>&& lf)
    : id{id},
      level{level},
      levelFactory{move(lf)},
      blockFactory{move(levelFactory->getBlockFactory(level))}
{
    activeBlock = blockFactory->next();
    nextBlock = blockFactory->next();
    statusEffects = levelFactory->getStatusEffects(level);
}
