#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include <array>
#include <map>
#include "grid.h"

class Block;

// Maintains a grid of cells, as well as a score associated with that grid
// Blocks can be added to the grid, and doing so will cause "tetris logic"
//  to be applied. This means:
//  - full lines will be cleared, causing above lines to "fall"
//  - score is updated
// Also includes a helper to check if a block is "causes collision" with 
//  this board.
class Board {
    public:
    static int const Width = 11;
    static int const Height = 18;

    private:
    Grid cells;
    std::map<int, int> idMap{};
    int score = 0;
    int numIds = 0;
    int level = 0;
    int totalRowsCleared = 0;

    int getNextId();
    void incrementId(int id);
    void decrementId(int id);
    int idCount(int id) const;

    void clearFullRows();
    void clearRow(int i);

    public:
    Board();
    Grid & getCells();
    void setLevel(int level);
    bool causesCollision(Block const &) const;
    void addBlock(Block const &);
    int getScore() const;
    int getTotalRowsCleared() const;
    void clearCell(int x, int y);
};


#endif
