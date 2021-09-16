#ifndef GAME_SETUP_H
#define GAME_SETUP_H

#include <vector>
#include <string>
#include <memory>

class CommandInterpreter;
class Game;

// A helper class for setting up a game of biquadris
class GameSetup {
    public:
    static bool GetArgs(int argc, char* argv[],
            std::vector<std::string>& defaultFiles,
            int& numPlayers,
            int& startLevel,
            bool& textMode,
            bool& extras);

    static void RegisterExtraCICommands(CommandInterpreter&);
    static void RegisterExtraGameCommands(Game*);
    static void RegisterExtraSpecialActions(Game*);

    static void RegisterBasicCICommands(CommandInterpreter&);
    static void RegisterBasicGameCommands(Game*);
    static void RegisterBasicSpecialActions(Game*);
};

#endif
