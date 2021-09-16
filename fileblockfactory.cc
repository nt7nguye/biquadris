#include "fileblockfactory.h"
#include "gameexceptions.h"
#include <vector>
#include "cell.h"
#include <fstream>
#include "blockfactory.h"
#include <string>
#include "cell.h"
#include "block.h"
#include <iostream>

using namespace std;

// Generates blocks in sequence from the file with 'filename'
// The sequence is looped after EOF
// Blocks are I, L, J, S, Z, O, T
//  so, a sample file might contain "I T O S S Z"
//
// Throws InvalidFile if the preconditions are not met
// Preconditions
//  - a file with 'filename' must exist
//  - the file contains at least one block
// Note: any strings in the file which are not blocks are ignored
FileBlockFactory::FileBlockFactory(int level, string filename) 
    : level{level}, nextIdx{0}
{
    ifstream file{filename};
    if (!file.is_open()) {
        // Failed to open file
        throw InvalidFile{};
    }

    string blockStr = "";
    while(file >> blockStr) {
        if (blockStr == "I") {
            blocks.emplace_back(CellType::I);
        }
        else if (blockStr == "L") {
            blocks.emplace_back(CellType::L);
        }
        else if (blockStr == "J") {
            blocks.emplace_back(CellType::J);
        }
        else if (blockStr == "S") {
            blocks.emplace_back(CellType::S);
        }
        else if (blockStr == "Z") {
            blocks.emplace_back(CellType::Z);
        }
        else if (blockStr == "O") {
            blocks.emplace_back(CellType::O);
        }
        else if (blockStr == "T") {
            blocks.emplace_back(CellType::T);
        }
    }

    if (blocks.size() == 0) {
        // File does not contain blocks
        throw InvalidFile{};
    }
}

// Returns the next block from the file
Block FileBlockFactory::next(int x, int y) {
    
    CellType type = blocks[nextIdx];

    //  CellType type = blocks[nextIdx];
    nextIdx = (nextIdx + 1) % blocks.size();

    if (type == CellType::I) {
        return getBlock(CellType::I, level, x, y);
    }
    else if (type == CellType::L) {
        return getBlock(CellType::L, level, x, y);
    }
    else if (type == CellType::J) {
        return getBlock(CellType::J, level, x, y);
    }
    else if (type == CellType::S) {
        return getBlock(CellType::S, level, x, y);
    }
    else if (type == CellType::Z) {
        return getBlock(CellType::Z, level, x, y);
    }
    else if (type == CellType::O) {
        return getBlock(CellType::O, level, x, y);
    }
    return getBlock(CellType::T, level, x, y);

}
