#ifndef RANDOM_BLOCK_FACTORY_H
#define RANDOM_BLOCK_FACTORY_H

#include "blockfactory.h"
#include <array>

// Distribution is a list of 7 numbers
// They represent the probability (relative to others) of each block
// The probabilities are matched up with blocks alphabetically
//  ie, { I, J, L, O, S, T, Z }
// 
// For example, { 1, 2, 2, 2, 2, 2, 2 } means block I is half as likely to be
//  chosen as any other block
typedef std::array<unsigned, 7> Distribution;

class Block;

// Creates blocks according to a provided probability distribution
class RandomBlockFactory : public BlockFactory {
    int level;
    Distribution dist;
    int maxRand = 0;

    public:
    RandomBlockFactory(int level, Distribution dist);
    Block next(
            int x = BlockFactory::SpawnX, 
            int y = BlockFactory::SpawnY) override;
};

#endif
