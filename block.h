#ifndef BLOCK_H
#define BLOCK_H

#include<vector>
#include "cell.h"

typedef std::vector<std::vector<bool>> CellMask;

class Block {
    CellMask mask;
    CellType type;
    int level;
    int xpos;
    int ypos;
    int maskWidth;
    int maskHeight;

    public:
    Block();
    Block(CellMask mask, CellType type, int level, int x, int y);
    CellMask const & getMask() const;
    CellType getType() const;
    int getLevel() const;
    int getXPos() const;
    void setXPos(int);
    void setYPos(int);
    int getYPos() const;
    int getWidth() const;
    int getHeight() const;
    
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void rotateClockwise();
    void rotateCounterClockwise();
};
#endif
