#include "block.h"
#include <vector>
#include "cell.h"

using namespace std;

Block::Block() {}

// Precondition:  mask must be a rectangle (each vector<bool> has same size)
Block::Block(CellMask mask, CellType type, int level, int x, int y)
    : mask{mask}, type{type}, level{level}, xpos{x}, ypos{y}
{
    maskHeight = mask.size();
    maskWidth = mask.size() >= 1 ? mask.at(0).size(): 0;
}

CellMask const & Block::getMask() const {
    return mask;
}

CellType Block::getType() const {
    return type;
}

int Block::getLevel() const {
    return level;
}

int Block::getXPos() const {
    return xpos;
}

int Block::getYPos() const {
    return ypos;
}

void Block::setXPos(int x) {
    xpos = x;
}

void Block::setYPos(int y) {
    ypos = y;
}

int Block::getWidth() const {
    return maskWidth;
}

int Block::getHeight() const {
    return maskHeight;
}

void Block::moveLeft() {
    --xpos;
}

void Block::moveRight() {
    ++xpos;
}

void Block::moveUp() {
    ++ypos;
}

void Block::moveDown() {
    --ypos;
}

// Rotates the mask 90degrees counter clockwise
// The position of the bottom left corner is invariant
void Block::rotateCounterClockwise() {
    CellMask newMask;
    for (int x = 0; x < maskWidth; ++x) {
        vector<bool> newRow;
        for (int y = maskHeight - 1; y >= 0; --y) {
            newRow.emplace_back(mask[y][x]);
        }
        newMask.emplace_back(newRow);
    }
    mask = newMask;
    int temp = maskHeight;
    maskHeight = maskWidth;
    maskWidth = temp;
}

// Rotates the mask 90degrees clockwise
// The position of the bottom left corner is invariant
void Block::rotateClockwise() {
    CellMask newMask;
    for (int x = maskWidth - 1; x >= 0; --x) {
        vector<bool> newRow;
        for (int y = 0; y < maskHeight; ++y) {
            newRow.emplace_back(mask[y][x]);
        }
        newMask.emplace_back(newRow);
    }
    mask = newMask;
    int temp = maskHeight;
    maskHeight = maskWidth;
    maskWidth = temp;
}
