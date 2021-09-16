#include "board.h"

#include "cell.h"
#include "block.h"
#include <iostream>

using namespace std;

Board::Board() {
    // init with empty cells
    for (int y = 0; y < Height; ++y) {
        for (int x = 0; x < Width; ++x) {
            cells[y][x] = Cell{CellType::Empty};
        }
    }
}

int Board::getNextId() {
    int nextId = numIds;
    ++numIds;
    return nextId;
}

Grid & Board::getCells() {
    return cells;
}

void Board::setLevel(int newLevel) {
    level = newLevel;
}

int Board::getTotalRowsCleared() const {
    return totalRowsCleared;
}

// Returns false if the block
//  1. fully overlaps the board (hence is not "off" the board), and
//  2. only overlaps cells that are Empty
bool Board::causesCollision(Block const & block) const {
    // Check if block is on the board
    if (block.getXPos() < 0 //off left edge
            || block.getXPos() + block.getWidth() > Width //off right edge
            || block.getYPos() < 0 //off bottom
            || block.getYPos() + block.getHeight() > Height //off top
       ){
        return true;
    }

    // Check if block overlaps non-empty square
    for (int x = 0; x < block.getWidth(); ++x) {
        for (int y = 0; y < block.getHeight(); ++y) {
            if (!block.getMask()[y][x]) continue; //this part of mask is empty

            //mask[y][x] is not empty: check if it causes overlap
            int cellX = block.getXPos() + x;
            int cellY = block.getYPos() + y;
            CellType overlappedCell = cells[cellY][cellX].getType();
            if (overlappedCell != CellType::Empty) {
                return true;
            }
        }
    }
    
    // No collisions
    return false;
}

// Updates the board's cells according to the block's position
// Any cells that would be covered by the block's mask get
//  changed to reflect the block's type
// After block has been added, rows are cleared and score is updated
void Board::addBlock(Block const & block) {
    int blockId = getNextId();
    for (int x = 0; x < block.getWidth(); ++x) {
        for (int y = 0; y < block.getHeight(); ++y) {
            int cellX = block.getXPos() + x;
            int cellY = block.getYPos() + y;

            bool cellIsOnBoard =
                cellX >= 0 && cellX < Width && cellY >= 0 && cellY < Height;
            bool blockCoversCell = block.getMask()[y][x];
            if (cellIsOnBoard && blockCoversCell) {
                cells[cellY][cellX] = Cell{
                    block.getType(), 
                    blockId,
                    block.getLevel()
                };
                incrementId(blockId);
            }
        }
    }
    clearFullRows();
}

int Board::getScore() const {
    return score;
}

void Board::incrementId(int id) {
    if (idMap.find(id) == idMap.end()) {
        // id is not in map: add it and set count to one
        idMap.emplace(id, 1);
    }
    else {
        // otherwise increment
        ++idMap[id];
    }
}

void Board::decrementId(int id) {
    if (idMap.find(id) != idMap.end()) {
        --idMap[id];
        if (idMap[id] == 0) {
            idMap.erase(id);
        }
    }
}

int Board::idCount(int id) const {
    return idMap.find(id) != idMap.end() ? idMap.at(id) : 0;
}


// Clears any completed rows, and updates the score to reflect
//  those changes
void Board::clearFullRows() {
    int rowsCleared = 0;
    for (int y = 0; y < Height; ++y) {
        // Check if row full
        bool full = true;
        for (int x = 0; x < Width; ++x) {
            if (cells[y][x].getType() == CellType::Empty) {
                full = false;
                break;
            }
        }

        if (full) {
            // Row is full: clear it
            clearRow(y);
            ++rowsCleared;
            --y;
        }
    }

    // Score points
    if (rowsCleared > 0) {
        score += (level + rowsCleared) * (level + rowsCleared);
    }
    totalRowsCleared += rowsCleared;
}

// Clears the ith row and moves all cells above the ith row down
void Board::clearRow(int i) {
    // Clear the row
    for (int x = 0; x < Width; ++x) {
        clearCell(x, i);
    }

    // Move blocks down
    for (int y = i; y < Height - 1; ++y) {
        for (int x = 0; x < Width; ++x) {
            cells[y][x] = Cell{cells[y+1][x]};
        }
    }
    for (int x = 0; x < Width; ++x) {
        cells[Height - 1][x] = Cell{CellType::Empty};
    }
}

// Makes the cell at (x,y) Empty
// Updates score if doing this removes the last cell from a dropped block
void Board::clearCell(int x, int y) {
    Cell& cell = cells[y][x];
    CellType type = cell.getType();
    if (type != CellType::Empty) {
        // Cell is covered by a block
        int groupId = cell.getGroupId();
        decrementId(groupId);

        if (idCount(groupId) == 0 && type != CellType::Garbage) {
            // block has been cleared: score points!
            int lvl = cell.getLevel();
            score += (lvl + 1) * (lvl + 1);
        }
    }

    // Clear the cell
    cells[y][x] = Cell{CellType::Empty};
}
