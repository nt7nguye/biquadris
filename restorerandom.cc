#include "restorerandom.h"
#include <vector>
#include <string>
#include "player.h"

using namespace std;

// Sets the player's block generator to be the level's block generator
void RestoreRandom::run(Player& player, int, std::vector<std::string>) {
    player.blockFactory = player.levelFactory->getBlockFactory(player.level);
}
