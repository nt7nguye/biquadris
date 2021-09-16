#ifndef GRID_H
#define GRID_H

#include <array>

class Cell;

// Typedef used for a grid of cells
// We put this in it's own file so that it can be imported
//  without creating unecessary dependencies.
typedef std::array<std::array<Cell, 11>, 18> Grid;

#endif
