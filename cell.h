#ifndef CELL_H
#define CELL_H

enum class CellType{ Empty, Garbage, I, J, L, S, Z, T, O };

// A cell for a biquadris board. 
// Mostly a data class which maintains information about the cell
class Cell {
    CellType type;
    int groupId;
    int level;

    public:
    Cell(CellType type = CellType::Empty, int groupId = 0, int level = 0);
    CellType getType() const;
    int getGroupId() const;
    int getLevel() const;
};

#endif
