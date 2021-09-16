#ifndef GRAPHICS_DISPLAY_H
#define GRAPHICS_DISPLAY_H

#include "observer.h"
#include <iostream>
#include <string>
#include <map>
#include "window.h"
#include <vector>
#include "game.h"

enum class CellType;

int getDisplayWidth(const Game&);

// Draws a graphical version of Biquadris to a window.
// Capable of handling multiple players by increasing the window width
//  to accomodate additional boards
// Uses RAII (hence the widow only exists until the display is destroyed)
class GraphicsDisplay: public Observer {
    public:
    static const int height;
    static const int boardWidth;
    static const int boardPadding;

    private:
    static std::map<CellType, Xwindow::colors> const colorMap;

    Xwindow window;
    Game* game;

    bool initialized = false;
    // 2 3D vectors of CellTypes storing past and current CellType info
    std::vector<std::vector<std::vector<CellType>>> beforeCellTypes{
        game->getPlayers().size(),
        std::vector<std::vector<CellType>>{
            Board::Height,
            std::vector<CellType>{
                Board::Width,
                CellType{}
            }
        }
    };
    std::vector<std::vector<std::vector<CellType>>> afterCellTypes{
        game->getPlayers().size(),
        std::vector<std::vector<CellType>>{
            Board::Height,
            std::vector<CellType>{
                Board::Width,
                CellType{}
            }
        }
    };

    Xwindow::colors getColorForType(CellType type) const;
    void update();

    public:
    GraphicsDisplay(Game*);
    void notify() override;
};

#endif
