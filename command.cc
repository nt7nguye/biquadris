#include "command.h"

// By default, commands have a 'None' move type
// None renamed to NoneMove due to X11 conflict

Move Command::moveType() const {
    return Move::NoneMove;
}
