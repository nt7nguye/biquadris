#ifndef BLOCK_FACTORY_H
#define BLOCK_FACTORY_H

enum class CellType;
class Block;


class BlockFactory {
    public:
    // Default coords for newly created blocks
    static int SpawnX;
    static int SpawnY;

    virtual Block next(int x = SpawnX, int y = SpawnY) = 0;
    virtual Block getBlock(CellType type, int level, int x, int y);
    virtual ~BlockFactory() = default;
};

#endif
