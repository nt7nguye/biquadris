#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "observer.h"
#include <iostream>
#include <map>

class Game;
enum class CellType;

// Outputs a text version of the Game to an output stream.
// Observes the Game it is attached to, and updates when the
//  game state changes.
class TextDisplay : public Observer {
    static std::map<CellType, char> const charMap;
    std::ostream& out;
    Game* game;

    char getCharForType(CellType type) const;
    void update();

    public:
    TextDisplay(std::ostream&, Game*);
    void notify() override;
};

#endif
