#include "cell.h"

Cell::Cell(CellType type, int groupId, int level) 
    : type{type}, groupId{groupId}, level{level} {}

CellType Cell::getType() const {
    return type;
}

int Cell::getGroupId() const {
    return groupId;
}

int Cell::getLevel() const {
    return level;
}
