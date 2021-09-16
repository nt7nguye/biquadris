#include "blockfactory.h"
#include "block.h"
#include "cell.h"

int BlockFactory::SpawnX = 0;
int BlockFactory::SpawnY = 14;

Block BlockFactory::getBlock(CellType type, int level, int x, int y) {
    CellMask mask;
    switch(type) {
        case CellType::Empty:
        case CellType::Garbage:
            mask = CellMask{{true}};
            break;
        case CellType::S:
            mask = CellMask{{true, true, false}, {false, true, true}};
            break;
        case CellType::Z:
            mask = CellMask{{false, true, true}, {true, true, false}};
            break;
        case CellType::T:
            mask = CellMask{{false, true, false}, {true, true, true}};
            break;
        case CellType::L:
            mask = CellMask{{true, true}, {true, false}, {true, false}};
            break;
        case CellType::J:
            mask = CellMask{{true, true}, {false, true}, {false, true}};
            break;
        case CellType::O:
            mask = CellMask{{true, true}, {true, true}};
            break;
        case CellType::I:
            mask = CellMask{{true},{true},{true},{true}};
            break;
    }
    return Block{mask, type, level, x, y};
}
