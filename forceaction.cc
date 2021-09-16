#include "forceaction.h"
#include "statuseffect.h"
#include "heavyeffect.h"
#include "player.h"
#include "gameexceptions.h"
#include <vector>
#include <string>
#include <memory>

using namespace std;

void ForceAction::run(
        Player& originalPlayer, 
        Player& affectedPlayer, 
        vector<string> const& args) 
{
    // Argument check
    CellType type;
    if (args.size() == 0) {
        throw InvalidCommand{};
    }
    
    string value = args[0];
    if (value == "Garbage") {
        type = CellType::Garbage;
    } else if (value == "S") {
        type = CellType::S;
    } else if (value == "Z") {
        type = CellType::Z;
    } else if (value == "T") {
        type = CellType::T;
    } else if (value == "L") {
        type = CellType::L;
    } else if (value == "J") {
        type = CellType::J;
    } else if (value == "O") {
        type = CellType::O;
    } else if (value == "I") {
        type = CellType::I;
    } else {
        throw InvalidCommand{};
    }
    
    // Take the blockFactory out and put back
    unique_ptr<BlockFactory> blockFactory = move(affectedPlayer.blockFactory);

    Block block = blockFactory->getBlock(
            type, 
            affectedPlayer.level, 
            blockFactory->SpawnX, 
            blockFactory->SpawnY);
    Board& board = affectedPlayer.board;
    affectedPlayer.blockFactory = move(blockFactory);
    
    // Check for collision, if does then the other player automatically loses.
    if (board.causesCollision(block)) {
        affectedPlayer.lost = true;
    } else {
        affectedPlayer.activeBlock = block;
    }
}

string ForceAction::getDesc() const {
    return "Force opponent to play a block of your choice";
}
