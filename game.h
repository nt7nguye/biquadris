#ifndef GAME_H
#define GAME_H

#include <vector>
#include "player.h"
#include "subject.h"
#include "prefixmap.h"
#include "commandtokenizer.h"
#include <map>
#include <string>
#include "specialaction.h"
#include <memory>

typedef std::map<std::string, std::unique_ptr<SpecialAction>> ActionMap;
typedef std::map<std::string, std::unique_ptr<Command>> CommandMap;
typedef std::unique_ptr<Command> CommandPtr;

class Command;

// Responsible for running a game
// Maintains a list of players, and contains logic for running commands
//  and special actions which act on those players.
class Game : public Subject {
    public:
    static int const MinPlayers;
    static int const MaxPlayers;
    static int const MinLevel;
    static int const MaxLevel;

    private:
    static int const ClearsToTriggerAction;  //rows which must be cleared in 
                                             //a turn to trigger special action
    int highScore = 0;
    bool newHighScore = false; //true if the highScore has been beaten
                               // during the current game
    bool tiedHighScore = false; //true if the highScore has been tied but
                                // not beaten during the current game
    int currentPlayerIdx = 0; //who's turn
    int numPlayers = 0;
    int defaultLevel = 0;
    std::vector<std::string> defaultFiles;
    std::vector<Player> players; 

    bool getSpecialAction = false; //if true, then the next command must be a 
                                   // special action

    ActionMap actions;
    CommandMap commands;

    void initPlayers();
    void runCommand(
            std::string const& cmd,
            int count,
            std::vector<std::string> args,
            bool notify);
    void runAction(
            std::string const& cmd,
            int count,
            std::vector<std::string> args,
            bool notify);

    void updateHighScore();
    void onGameOver();
    void outputActionDescriptions();
    void applyEffectsBefore(CommandPtr&, Player&);
    void applyEffectsAfter(CommandPtr&, Player&);
    void applyEffectsBeforeTurn(Player&);
    void applyEffectsAfterTurn(Player&);
    void removeFinishedEffects(Player&);

    public:
    Game(int numPlayers, 
         int level, 
         std::vector<std::string> defaultFiles);

    // Run a command OR a special action
    void run(std::string const& cmd, int count, std::vector<std::string> args, bool notify);
    void restart();

    int getHighScore() const;
    bool gameIsOver() const;

    std::vector<Player> const & getPlayers() const;
    Player const & getCurrentPlayer() const;

    void registerAction(std::string name, std::unique_ptr<SpecialAction>&&);
    void registerCommand(std::string name, std::unique_ptr<Command>&&);
};

#endif
