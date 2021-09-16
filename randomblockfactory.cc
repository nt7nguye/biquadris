#include "randomblockfactory.h"
#include "block.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// Distribution is a list of 7 numbers
// They represent the probability (relative to others) of each block
// The probabilities are matched up with blocks alphabetically
//  ie, { I, J, L, O, S, T, Z }
// 
// For example, { 1, 2, 2, 2, 2, 2, 2 } means block I is half as likely to be
//  chosen as any other block
RandomBlockFactory::RandomBlockFactory(int level, Distribution dist)
    : level{level}, dist{dist}
{
    maxRand = 0;
    for (auto value : dist) {
        maxRand += value;
    }
}

// Return a random block using the probability distribution
Block RandomBlockFactory::next(int x, int y) {
    unsigned r = rand() % maxRand;
    unsigned sum = 0;
    for (unsigned i = 0; i < dist.size(); ++i) {
        sum += dist[i];
        if (sum > r) {
            switch (i) {
                case 0:
                    return getBlock(CellType::I, level, x, y);
                case 1:
                    return getBlock(CellType::J, level, x, y);
                case 2:
                    return getBlock(CellType::L, level, x, y);
                case 3:
                    return getBlock(CellType::O, level, x, y);
                case 4:
                    return getBlock(CellType::S, level, x, y);
                case 5:
                    return getBlock(CellType::T, level, x, y);
            }
        }
    }
    return getBlock(CellType::Z, level, x, y);

}
