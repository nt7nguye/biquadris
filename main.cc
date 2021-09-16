#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <memory>
#include "gamesetup.h"
#include "commandinterpreter.h"
#include "cell.h"
#include "game.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"
#include "window.h"
#include "gameexceptions.h"
#include "command.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Handle command line arguments
    vector<string> defaultFiles{
        "biquadris_sequence1.txt", 
        "biquadris_sequence2.txt"};
    int numPlayers = 2;
    int startLevel = 0;
    bool textMode = false;
    bool extras = false;
    bool success = GameSetup::GetArgs(argc, argv,
            defaultFiles, numPlayers, startLevel, textMode, extras);

    if (!success) {
        // Failed to parse args: abort
        return 1;
    }

    try {
        // Create a game
        unique_ptr<Game> game{new Game{numPlayers, startLevel, defaultFiles}};
        CommandInterpreter commandInterpreter{game.get()};

        // Register commands and actions
        GameSetup::RegisterBasicCICommands(commandInterpreter);
        GameSetup::RegisterBasicGameCommands(game.get());
        GameSetup::RegisterBasicSpecialActions(game.get());
        if (extras) {
            GameSetup::RegisterExtraCICommands(commandInterpreter);
            GameSetup::RegisterExtraGameCommands(game.get());
            GameSetup::RegisterExtraSpecialActions(game.get());
        }

        // Attach displays
        vector<unique_ptr<Observer>> displays{};
        displays.emplace_back(move(
                unique_ptr<Observer>{ new TextDisplay{cout, game.get()} }
                ));
        if (!textMode) {
            displays.emplace_back(move(
                    unique_ptr<Observer>{ new GraphicsDisplay{game.get()} }
                    ));
        }

        // Game loop
        string command;
        while (getline(cin, command)) {
            commandInterpreter.run(command);
            if (game->gameIsOver()) break;
        }
    }
    catch (InvalidLevel) {
        cerr << "Invalid level: must be in range "
            << Game::MinLevel << " to " << Game::MaxLevel << endl;
        return 1;
    }
    catch (UnsupportedPlayerCount) {
        cerr << "Invalid player count: must be in range "
            << Game::MinPlayers << " to " << Game::MaxPlayers << endl;
        return 1;
    }
    catch (InvalidFile) {
        cerr << "Invalid script file" << endl;
        return 1;
    }

    cout << "Exiting program..." << endl;
    return 0;
}
