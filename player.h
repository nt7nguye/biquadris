#ifndef PLAYER_H
#define PLAYER_H

#include "block.h"
#include "board.h"
#include <vector>
#include <string>
#include "blockfactory.h"
#include "levelfactory.h"
#include "statuseffect.h"
#include <memory>

// A player is a collection of objects
// Player's do not have any internal logic. Player's have no invariants,
//  though invariants may be imposed upon them by an owner (eg. a Game)
//
// Dev note: a player could have been a collection of arrays on Game
//  Eg., Game could have had private vectors containing boards, blocks,
//  status effects, etc...
//  These could have been tied together with an index
//  Eg., board[0], activeBlock[0], ... all belong to player 1
// Doing that seemed messier than bundling up all the associated data in 
//  a struct.
struct Player {
    int id;
    int turn = 0;
    bool isBlind = false;
    int level = 0;
    bool turnIsDone = false;
    bool lost = false;
    Board board;
    Block activeBlock;
    Block nextBlock;
    std::vector<std::unique_ptr<StatusEffect>> statusEffects;
    std::unique_ptr<LevelFactory> levelFactory;
    std::unique_ptr<BlockFactory> blockFactory;

    Player(int id, int level, std::unique_ptr<LevelFactory>&&);
};

#endif
