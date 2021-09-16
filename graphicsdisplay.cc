#include "graphicsdisplay.h"
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
#include <string>
#include "window.h"

using namespace std;

const int GraphicsDisplay::height = 600;
const int GraphicsDisplay::boardWidth = 220;
const int GraphicsDisplay::boardPadding = 40;

int getDisplayWidth(const Game& game) {
    int numberOfPlayers = game.getPlayers().size();
    int width = (GraphicsDisplay::boardWidth*numberOfPlayers) +
                (GraphicsDisplay::boardPadding*(numberOfPlayers+1));
    return width;
}

map<CellType, Xwindow::colors> const GraphicsDisplay::colorMap = {
    { CellType::Garbage, Xwindow::Brown },
    { CellType::Empty, Xwindow::White },
    { CellType::I, Xwindow::Red },
    { CellType::J, Xwindow::Green },
    { CellType::L, Xwindow::DarkGreen },
    { CellType::S, Xwindow::Cyan },
    { CellType::Z, Xwindow::Yellow },
    { CellType::T, Xwindow::Magenta },
    { CellType::O, Xwindow::Orange },
};

GraphicsDisplay::GraphicsDisplay(Game* game)
    : window{ Xwindow{getDisplayWidth(*game), GraphicsDisplay::height} },
      game{game}
{
    game->attach(this);
    notify();
}

Xwindow::colors GraphicsDisplay::getColorForType(CellType type) const {
    if (colorMap.find(type) == colorMap.end()) {
        // Not found
        return Xwindow::White;
    }
    return colorMap.at(type);
}

void GraphicsDisplay::update() {
    // getting PLayer info ...
    // ... all Players ...
    vector<Player> const& players = game->getPlayers();
    int numberOfPlayers = players.size();

    // ... set all Boards ...
    vector<Board> boards;
    for (auto &player: players) {
        boards.emplace_back(player.board);
    }

    // ... current Player ...
    const Player& currentPlayer = game->getCurrentPlayer();

    // ... and next Player
    const int nextPlayerId = (currentPlayer.id+1 > numberOfPlayers)
        ? 1
        : currentPlayer.id+1;
    vector<vector<vector<CellType>>> nextCellTypes;
    for (auto& player : players) {
        nextCellTypes.emplace_back(vector<vector<CellType>>{});
        int idx = player.id - 1;
        Block const& nb = player.nextBlock;
        for (int y = 3; y >= 0; --y) {
            nextCellTypes[idx].emplace_back(vector<CellType>{});
            for (int x = 0; x < 4; ++x) {
                int dy = y - (4 - nb.getHeight());
                if (dy >= 0
                        && dy < nb.getHeight() 
                        && x < nb.getWidth()
                        && nb.getMask()[dy][x]) 
                {
                    nextCellTypes[idx][3-y].emplace_back(nb.getType());
                } else {
                    nextCellTypes[idx][3-y].emplace_back(CellType::Empty);
                }
            }
        }
    }

    // setting our CellType 3D vectors ...
    // ... to the Cells dropped already ...
    int playerIdx = 0;
    for (auto board: boards) {
        for (int y = 0; y < Board::Height; ++y) {
            for (int x = 0; x < Board::Width; ++x) {
                afterCellTypes[playerIdx][y][x] =
                    board.getCells()[y][x].getType();
            }
        }
        ++playerIdx;
    }

    // ... and to the current Block Cells
    for (auto& player : players) {
        Block const& activeBlock = player.activeBlock;
        CellMask const& mask = player.activeBlock.getMask();
        for (int y = 0; y < (int)mask.size(); ++y) {
            for (int x = 0; x < (int)mask[y].size(); ++x) {
                if (mask[y][x]) {
                    int cellIdx = player.id - 1;
                    int celly = y + activeBlock.getYPos();
                    int cellx = x + activeBlock.getXPos();
                    afterCellTypes[cellIdx][celly][cellx] = 
                        activeBlock.getType();
                }
            }
        }
    }

    // blind effect modifier
    for (int playerIdx = 0; playerIdx < numberOfPlayers; ++playerIdx) {
        if (players.at(playerIdx).isBlind) {
            for (int y = 0; y < Board::Height; ++y) {
                for (int x = 0; x < Board::Width; ++x) {
                    if (y <= 11 && y >= 2 && x >= 2 && x <= 8)
                    afterCellTypes[playerIdx][y][x] =
                        CellType::Empty;
                }
            }
        }
    }

    // printing
    int boardWidth = 220; // width of Board; 11x20
    int boardHeight = 360; // height of Board; 18x20
    int cellWidth = 20;
    int cellHeight = 20;
    int xEnd = 40; // x-coordinate where printing begins
    int xIncrBoard = 40; // x-coordinate increment between Boards
    const int xEndStd = 40; // x-coordinate to reset xEnd to
    int yEnd = 20; // y-coordinate where printing begins
    int yIncr = 20; // y-coordinate increment

    const int maskHeight = 4;
    const int maskWidth = 4;

    // the high score
    int previousPlayerId = (currentPlayer.id-1 < 1)
        ? numberOfPlayers
        : currentPlayer.id-1;
    if (players[previousPlayerId-1].board.getScore() == game->getHighScore()
            || currentPlayer.board.getScore() == game->getHighScore()) 
    {
        xEnd = getDisplayWidth(*game)/2-xEndStd;
        string s = "High Score: " + to_string(game->getHighScore());
        window.fillRectangle(xEnd-xEndStd,
                             yEnd-yIncr/2,
                             boardWidth,
                             yIncr,
                             Xwindow::White
        );
        window.drawBigString(xEnd, yEnd, s, Xwindow::Black);
    }
    yEnd += yIncr*2;

    // the Player number
    xEnd = xEndStd;
    if (!initialized) {
        for (auto &p: players) {
            string s = "Player: " + to_string(p.id);
            window.drawBigString(xEnd, yEnd, s, Xwindow::Black);
            xEnd += boardWidth + xIncrBoard;
        }
    }
    yEnd += yIncr;

    // the levels
    xEnd = xEndStd;
    for (auto &p: players) {
        string s = "Level: " + to_string(p.level);
        window.fillRectangle(xEnd-xEndStd,
                             yEnd-yIncr/2,
                             boardWidth,
                             yIncr,
                             Xwindow::White
        );
        window.drawBigString(xEnd, yEnd, s, Xwindow::Black);
        xEnd += boardWidth + xIncrBoard;
    }
    yEnd += yIncr;

    // the scores
    xEnd = xEndStd;
    for (auto &p: players) {
        string s = "Score: " + to_string(p.board.getScore());
        window.fillRectangle(xEnd-xEndStd,
                             yEnd-yIncr/2,
                             boardWidth,
                             yIncr,
                             Xwindow::White
        );
        window.drawBigString(xEnd, yEnd, s, Xwindow::Black);
        xEnd += boardWidth + xIncrBoard;
    }
    yEnd += yIncr;

    // the Boards and their border lines ...
    xEnd = xEndStd;
    for (int playerIdx = 0; playerIdx < numberOfPlayers; ++playerIdx) {
        if (playerIdx != currentPlayer.id-1 || playerIdx != nextPlayerId-1) {
            // ... the Board
            for (int y = Board::Height-1; y >= 0; --y) {
                for (int x = 0; x < Board::Width; ++x) {
                    if (beforeCellTypes[playerIdx][y][x] !=
                        afterCellTypes[playerIdx][y][x]) {
                            window.fillRectangle(
                                xEnd + (cellWidth*x),
                                yEnd + (cellHeight*(Board::Height-y-1)),
                                cellWidth,
                                cellHeight,
                                getColorForType(afterCellTypes[playerIdx][y][x])
                            );
                    }
                }
            }
            // ... and the border lines
            window.drawLine(xEnd,
                            yEnd,
                            xEnd+boardWidth,
                            yEnd); // top
            window.drawLine(xEnd,
                            yEnd+boardHeight,
                            xEnd+boardWidth,
                            yEnd+boardHeight); // bottom
            window.drawLine(xEnd,
                            yEnd,
                            xEnd,
                            yEnd+boardHeight); // left
            window.drawLine(xEnd+boardWidth,
                            yEnd,
                            xEnd+boardWidth,
                            yEnd+boardHeight); // right
            window.drawLine(xEnd,
                            yEnd+(3*cellHeight),
                            xEnd+boardWidth,
                            yEnd+(3*cellHeight)); // demarcating line
            xEnd += boardWidth + xIncrBoard;
        }
    }
    yEnd += boardHeight + yIncr;

    // the "Next" section ...
    const int xNextOffset = 20;

    // ... the text first ...
    xEnd = xEndStd;
    for (auto& player : players) {
        window.fillRectangle(xEnd-xEndStd,
                             yEnd-yIncr/2,
                             boardWidth,
                             yIncr,
                             Xwindow::White
        );
        string text = "Next:";
        if (player.lost) text = "Next: LOST";
        window.drawBigString(xEnd, yEnd, text, Xwindow::Black);
        xEnd += boardWidth + xIncrBoard;
    }
    yEnd += yIncr;

    // ... and then the Next block
    xEnd = xEndStd;
    for (auto& player : players) {
        int i = player.id -1;
        for (int y = 0; y < maskHeight; ++y) {
            for (int x = 0; x < maskWidth; ++x) {
                if (player.id == currentPlayer.id) {
                    window.fillRectangle(
                            xEnd + (cellWidth*0.75*x) + xNextOffset,
                            yEnd + (cellHeight*0.75*y),
                            cellWidth*0.75, // smaller Cells
                            cellHeight*0.75, // smaller Cells
                            getColorForType(nextCellTypes[i][y][x])
                            );
                } else {
                    window.fillRectangle(
                            xEnd + (cellWidth*0.75*x) + xNextOffset,
                            yEnd + (cellHeight*0.75*y),
                            cellWidth*0.75, // smaller Cells
                            cellHeight*0.75, // smaller Cells
                            Xwindow::White
                    );
                }
             }
        }
        xEnd += boardWidth + xIncrBoard;
    }
    yEnd += yIncr;

    beforeCellTypes = afterCellTypes;

    // setting initialized to true after first print
    if (!initialized) {
        initialized = true;
    }
}

void GraphicsDisplay::notify() {
    update();
}
