#ifndef FILE_BLOCK_FACTORY_H
#define FILE_BLOCK_FACTORY_H

#include "blockfactory.h"
#include <fstream>
#include <vector>

enum class CellType;
class Block;

// Generates a sequence of blocks
class FileBlockFactory : public BlockFactory {
    int level;
    std::vector<CellType> blocks; //sequence of blocks to generate
    int nextIdx; //index of next block to generate

    public:
    FileBlockFactory(int level, std::string filename);
    Block next(
            int x = BlockFactory::SpawnX, 
            int y = BlockFactory::SpawnY) override;
};

#endif
