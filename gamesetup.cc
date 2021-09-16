#include "gamesetup.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <memory>
#include "commandinterpreter.h"
#include "game.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"
#include "window.h"
#include "specialaction.h"
#include "heavyaction.h"
#include "forceaction.h"
#include "blindaction.h"
#include "moveblock.h"
#include "dropblock.h"
#include "rotateblock.h"
#include "incrementlevel.h"
#include "norandom.h"
#include "restorerandom.h"
#include "transformblock.h"
#include "gameexceptions.h"
#include "pokeaction.h"

using namespace std;


// Registers extra commands to the command interpreter (bonus features)
void GameSetup::RegisterExtraCICommands(
        CommandInterpreter& commandInterpreter)
{
    commandInterpreter.registerCommand("poke");
    commandInterpreter.registerCommand(CommandInterpreter::Macro);
    commandInterpreter.registerCommand(CommandInterpreter::Rename);
}

// Registers an exta set of commands to the game (bonus features)
void GameSetup::RegisterExtraGameCommands(Game* game) {
    // None right now :(
}

// Registers an extra set of special actions to the game (bonus features)
void GameSetup::RegisterExtraSpecialActions(Game* game) {
    game->registerAction(
            "poke",
            unique_ptr<SpecialAction>{ new PokeAction{} }
            );
}

// Populates numPlayers, startLevel, textMode, extras from commandline args
// Returns true if parsing is successful
//         false otherwise
bool GameSetup::GetArgs(int argc, char* argv[],
        vector<string>& defaultFiles,
        int& numPlayers,
        int& startLevel,
        bool& textMode,
        bool& extras)
{
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-text") {
            textMode = true;
        }
        else if (arg == "-extras") {
            extras = true;
        }
        else if (arg == "-seed") {
            if (i + 1 < argc) {
                try {
                    srand(stol(argv[++i]));
                } catch (...) {
                    cerr << "Invalid argument: "
                        << "-seed expects an unsigned integer" << endl;
                    return false;
                }
            }
            else {
                cerr << "-seed option requires one argument" << endl;
                return false;
            }
        }
        else if (arg == "-scriptfile1") {
            if (i + 1 < argc) {
                defaultFiles[0] = argv[++i];
            }
            else {
                cerr << "-scriptfile1 option requires one argument" << endl;
                return false;
            }
        }
        else if (arg == "-scriptfile2") {
            if (i + 1 < argc) {
                defaultFiles[1] = argv[++i];
            }
            else {
                cerr << "-scriptfile2 option requires one argument" << endl;
                return false;
            }
        }
        else if (arg == "-startlevel") {
            if (i + 1 < argc) {
                try {
                    startLevel = stoi(argv[++i]);
                }
                catch (...) {
                    cerr << "Invalid argument: "
                        << "-startlevel expects an unsigned integer" << endl;
                    return false;
                }
            }
            else {
                cerr << "-startlevel option requires one argument" << endl;
                return false;
            }
        }
        else if (arg == "-players") {
            if (i + 1 < argc) {
                numPlayers = stoi(argv[++i]);
            }
            else {
                cerr << "-players option requires one argument" << endl;
                return false;
            }
        }
        else {
            cerr << "Invalid option: " << arg << endl;
            return false;
        }
    }
    
    // All args parsed succesfully
    return true;
}


// Registers a basic set of commands to the interpretter
void GameSetup::RegisterBasicCICommands(
        CommandInterpreter& commandInterpreter)
{
    commandInterpreter.registerCommand("left");
    commandInterpreter.registerCommand("right");
    commandInterpreter.registerCommand("down");

    commandInterpreter.registerCommand("clockwise");
    commandInterpreter.registerCommand("counterclockwise");
    
    commandInterpreter.registerCommand("drop");
    
    commandInterpreter.registerCommand("levelup");
    commandInterpreter.registerCommand("leveldown");
    commandInterpreter.registerCommand("norandom");
    commandInterpreter.registerCommand("random");
   
    commandInterpreter.registerCommand("heavy");
    commandInterpreter.registerCommand("force");
    commandInterpreter.registerCommand("blind");

    commandInterpreter.registerCommand("I");
    commandInterpreter.registerCommand("J");
    commandInterpreter.registerCommand("L");
    commandInterpreter.registerCommand("O");
    commandInterpreter.registerCommand("S");
    commandInterpreter.registerCommand("Z");
    commandInterpreter.registerCommand("T");
    commandInterpreter.registerCommand("Garbage");
}

// Registers a basic set of commands to the game
void GameSetup::RegisterBasicGameCommands(Game* game) {
  game->registerCommand(
            "left",
            unique_ptr<Command>{ new MoveBlock{MoveBlock::Direction::Left} }
            );
    game->registerCommand(
            "right",
            unique_ptr<Command>{ new MoveBlock{MoveBlock::Direction::Right} }
            );
    game->registerCommand(
            "down",
            unique_ptr<Command>{ new MoveBlock{MoveBlock::Direction::Down} }
            );
    game->registerCommand(
            "clockwise",
            unique_ptr<Command>{ 
                    new RotateBlock{RotateBlock::Direction::Clockwise} }
            );
    game->registerCommand(
            "counterclockwise",
            unique_ptr<Command>{ 
                    new RotateBlock{RotateBlock::Direction::CounterClockwise} }
            );
    game->registerCommand(
            "drop",
            unique_ptr<Command>{ new DropBlock{} }
            );
    game->registerCommand(
            "levelup",
            unique_ptr<Command>{ 
                    new IncrementLevel{IncrementLevel::Direction::Up} }
            );
    game->registerCommand(
            "leveldown",
            unique_ptr<Command>{ 
                    new IncrementLevel{IncrementLevel::Direction::Down} }
            );
    game->registerCommand(
            "norandom",
            unique_ptr<Command>{ new NoRandom{} }
            );
    game->registerCommand(
            "random",
            unique_ptr<Command>{ new RestoreRandom{} }
            );
    game->registerCommand(
            "I", 
            unique_ptr<Command>{new TransformBlock{CellType::I} }
            );
    game->registerCommand(
            "J",
            unique_ptr<Command>{new TransformBlock{CellType::J} }
            );
    game->registerCommand(
            "L",
            unique_ptr<Command>{new TransformBlock{CellType::L} }
            );
    game->registerCommand(
            "S",
            unique_ptr<Command>{new TransformBlock{CellType::S} }
            );
    game->registerCommand(
            "Z",
            unique_ptr<Command>{new TransformBlock{CellType::Z} }
            );
    game->registerCommand(
            "T",
            unique_ptr<Command>{new TransformBlock{CellType::T} }
            );
    game->registerCommand(
            "O",
            unique_ptr<Command>{new TransformBlock{CellType::O} }
            );
    game->registerCommand(
            "Garbage",
            unique_ptr<Command>{new TransformBlock{CellType::Garbage} }
            );
}

// Registers a basic set of special actions to the game
void GameSetup::RegisterBasicSpecialActions(Game* game) {
    game->registerAction(
            "heavy",
            unique_ptr<SpecialAction>{ new HeavyAction{} }
            );
    game->registerAction(
            "force",
            unique_ptr<SpecialAction>{ new ForceAction{} }
            );
    game->registerAction(
            "blind",
            unique_ptr<SpecialAction>{ new BlindAction{} }
            );
}
