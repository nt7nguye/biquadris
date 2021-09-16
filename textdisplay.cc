#include "textdisplay.h"
#include "observer.h"
#include "cell.h"
#include <map>
#include "block.h"
#include "board.h"
#include <vector>
#include <iostream>
#include "player.h"
#include "game.h"
#include <iomanip>
#include "block.h"

using namespace std;

map<CellType, char> const TextDisplay::charMap = {
    { CellType::Garbage, '*' },
    { CellType::Empty, ' ' },
    { CellType::I, 'I' },
    { CellType::J, 'J' },
    { CellType::L, 'L' },
    { CellType::S, 'S' },
    { CellType::Z, 'Z' },
    { CellType::T, 'T' },
    { CellType::O, 'O' },
};

TextDisplay::TextDisplay(ostream& out, Game* game) : out{out}, game{game} {
    game->attach(this);
    notify();
}

char TextDisplay::getCharForType(CellType type) const {
    if (charMap.find(type) == charMap.end()) {
        // Not found
        return '~';
    }
    return charMap.at(type);
}

void TextDisplay::update() {
    // get data for all Players
    // vector of Players
    vector<Player> const& players = game->getPlayers();
    // vector of Boards
    vector<Board> boards;
    for (auto &p: players) {
        boards.emplace_back(p.board);
    }

    int numberOfPlayers = players.size();

    // vector for each Player of 2D vector of CellTypes
    vector<vector<vector<CellType>>> cellTypes{players.size(),
    vector<vector<CellType>>{Board::Height,
    vector<CellType>{Board::Width, CellType{}}}};

    // get active Player and Block, and next Block to print on screen
    Player const& currentPlayer = game->getCurrentPlayer();

    // sets the Cells on the display to the Board's Cells
    int boardIdx = 0;
    for (auto &board: boards) {
        for (int y = 0; y < Board::Height; ++y) {
            for (int x = 0; x < Board::Width; ++x) {
                cellTypes[boardIdx][y][x] = board.getCells()[y][x].getType();
            }
        }
        ++boardIdx;
    }
    // sets the Cells on the display to the current Block's Cells
    for (auto& player : players) {
        Block const& activeBlock = player.activeBlock;
        CellMask const& mask = player.activeBlock.getMask();
        for (int y = 0; y < (int)mask.size(); ++y) {
            for (int x = 0; x < (int)mask[y].size(); ++x) {
                if (mask[y][x]) {
                    int cellIdx = player.id - 1;
                    int celly = y + activeBlock.getYPos();
                    int cellx = x + activeBlock.getXPos();
                    cellTypes[cellIdx][celly][cellx] = activeBlock.getType();
                }
            }
        }
    }

    // does the actual printing
    // break line
    int breakCount = numberOfPlayers * 11 + (numberOfPlayers - 1) * 7;
    out << setfill('=') << setw(breakCount) << "" << endl; 
    
    // highscore
    // Leave 15 spaces to center the score
    int centerCount = (breakCount - 15) / 2;
    out << setfill(' ') << setw(centerCount) << "";
    out << "High Score: " << game->getHighScore() << endl;
    
    // break line
    out << setfill('=') << setw(breakCount) << "" << endl;
    out.fill(' ');

    // the levels
    for (auto &p: players) {
        out << "Level: " << setw(4) << p.level << "       ";
    }
    out << endl;
    // the scores
    for (auto &p: players) {
        out << "Score: " << setw(4) << p.board.getScore() << "       ";
    }
    out << endl;
    // the dividers
    for (int i = 0; i < numberOfPlayers; ++i) {
        out << "-----------" << "       ";
    }
    out << endl;
    // the Boards
    for (int y = Board::Height - 1; y >= 0; --y) {
        for (int boardIdx = 0; boardIdx < numberOfPlayers; ++boardIdx) {
            for (int x = 0; x < Board::Width; ++x) {
                if (players.at(boardIdx).isBlind && 
                    y <= 11 && y >= 2 
                    && x >= 2 && x <= 8) {
                        out << '?';
                } else {
                        out << getCharForType(cellTypes[boardIdx][y][x]);
                }
            }
            out << "       ";
        }
        out << endl;
    }
    // the dividers
    for (int i = 0; i < numberOfPlayers; ++i) {
        out << "-----------" << "       ";
    }
    out << endl;
    // "Next" Block
    for (auto& player : players) {
        if (player.lost) {
            out << "LOST:             ";
        }
        else {
            out << "Next:             ";
        }
    }
    out << endl;

    for(int y = 3; y >= 0; --y) {
        for (auto& player : players) {
            Block const& nb = player.nextBlock;
            if (player.id == currentPlayer.id) {
                for (int x = 0; x < 4; ++x) {
                    int dy = y - (4 - nb.getHeight());
                    if (dy >= 0
                            && dy < nb.getHeight() 
                            && x < nb.getWidth()
                            && nb.getMask()[dy][x]) 
                    {
                        out << getCharForType(nb.getType());
                    } else {
                        out << " ";
                    }
                }
                out << "              ";
            } else {
                out << "                  ";
            }
        }
        out << endl;
    }

    // break line
    out << setfill('=') << setw(breakCount) << "" << endl;
}


void TextDisplay::notify() {
    update();
}
