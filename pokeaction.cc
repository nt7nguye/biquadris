#include "pokeaction.h"
#include <memory>
#include <vector>
#include <string>
#include "player.h"
#include "board.h"
#include "grid.h"
#include "cell.h"
#include <cstdlib>

using namespace std;

void PokeAction::run(
        Player& originalPlayer, 
        Player& affectedPlayer, 
        vector<string> const& args) 
{
    Board& board = affectedPlayer.board;
    Grid& cells = board.getCells();

    // Determine which cells contain blocks that could be "poked"
    vector<vector<int>> pokeable;
    for (int y = 0; y < Board::Height - 1; ++y) {
        for (int x = 0; x < Board::Width - 1; ++x) {
            CellType type = cells[y][x].getType();
            if (type != CellType::Empty && type != CellType::Garbage) {
                // cell can be poked
                vector<int> posn{x, y};
                pokeable.emplace_back(posn);
            }
        }
    }

    // Poke up to 3 of the cells
    for (int i = 0; i < 3; ++i) {
        if (pokeable.size() == 0) break;
        int poke = rand() % pokeable.size();
        vector<int> pos = pokeable[poke];
        board.clearCell(pos[0], pos[1]);
    }
}

string PokeAction::getDesc() const {
    return "Causes up to three cells to be removed from next player's board";
}
