#include "game.h"
#include <iostream>
#include "player.h"
#include "command.h"
#include "fileblockfactory.h"
#include "levelfactory.h"
#include <memory>
#include "prefixmap.h"
#include "commandtokenizer.h"
#include "specialaction.h"
#include "gameexceptions.h"

using namespace std;

int const Game::MinPlayers = 2;
int const Game::MaxPlayers = 8;
int const Game::MinLevel = LevelFactory::MinLevel;
int const Game::MaxLevel = LevelFactory::MaxLevel;
int const Game::ClearsToTriggerAction = 2;


// Starts a new game
//  numPlayers: how many of players are in the game
//  level: the starting level for new games
//  defaultFiles: files to get block for level 0
// Throws
//  UnsupportedPlayerCount if < 2 players are requested
//  MissingDefaultFiles    if < 1 defaultFiles are provided 
Game::Game(int numPlayers, int level, vector<string> defaultFiles)
    : numPlayers{numPlayers}, defaultLevel{level}, defaultFiles{defaultFiles}
{
    if (numPlayers < Game::MinPlayers || numPlayers > Game::MaxPlayers) {
        throw UnsupportedPlayerCount{};
    }
    if (defaultFiles.size() == 0) throw MissingDefaultFiles{}; 

    initPlayers();
}

// Initializes the players array
// Players are all set to start at the 'defaultLevel'
void Game::initPlayers() {
    players.clear();
    for (int i = 0; i < numPlayers; ++i) {
        string file = defaultFiles[ i % defaultFiles.size() ];
        players.emplace_back(Player{
                i + 1, defaultLevel,
                unique_ptr<LevelFactory>{ new LevelFactory{ file } }
                });
        players[i].board.setLevel(defaultLevel);
    }
}

// Calling this function advances the state of the game according
//  to the provided command.
void Game::run(string const& cmdstr, int count, vector<string> args, bool notify) {
    // Run the command
    if (getSpecialAction) {
        runAction(cmdstr, count, args, notify);
    }
    else {
        runCommand(cmdstr, count, args, notify);
    }

    // Update the highscore and check if game is over
    updateHighScore();
    if (gameIsOver()) {
        onGameOver();
    }
}

// Updates the highscore based on the current player scores
// Keeps track of whether the current highscore has been broken or tied
//  during this game
void Game::updateHighScore() {
    int best = 0;
    for (auto & player : players) {
        int score = player.board.getScore();
        if (score > best) {
            best = score;
        }
    }

    if (best > highScore) {
        highScore = best;
        newHighScore = true;
        tiedHighScore = false;
    }
    else if (best == highScore && !newHighScore) {
        tiedHighScore = true;
    }
}

// Should be called when game ends.
// Outputs game results and asks if you want to play again
void Game::onGameOver() {
    // Who won?
    int bestScore = 0;   //what is the best score?
    int survivedId = 1;  //who made it to the end?
    for (auto& player : players) {
        int score = player.board.getScore();
        if (!player.lost) survivedId = player.id;
        if (score > bestScore) bestScore = score;
    }

    // Output results
    cout << endl;
    cout << "Game Over." << endl;
    cout << "High Score: " << highScore << endl;
    cout << "Final scores:" << endl;
    for (auto& player : players) {
        int score = player.board.getScore();
        cout << "  P" << player.id << ": " << score << " points";
        if (score == bestScore && newHighScore) {
            cout << " NEW HIGHSCORE!";
        }
        else if (score == bestScore && tiedHighScore) {
            cout << " TIED HIGHSCORE!";
        }
        cout << endl;
    }
    cout << "P" << survivedId << " survived the longest!" << endl;
    cout << endl;
    cout << "Play again? (y/n)" << endl;

    // Check if game should play again or exit
    string ans;
    while(cin >> ans) {
        if (ans == "y") {
            restart();
            break;
        }
        else if (ans == "n") {
            exit(1);
        }
        else {
            cout << "Invalid Input. Play Again? (y/n)" << endl;
        }
    }
}

// Runs a command
// Note: commands act on players, and update their internal state
//  Status Effects will also be applied to the current player
//  If running the command ends the player turn, then the active block
//   is added to the player board, and control moves to the next player
void Game::runCommand(string const& cmdstr, int count, vector<string> args, bool notify) {
    if (commands.find(cmdstr) == commands.end()) {
        // Command not found
        throw InvalidCommand{};
    }
    unique_ptr<Command>& cmd = commands[cmdstr];

    // Get current player
    Player& player = players.at(currentPlayerIdx);
    int oldRowsCleared = player.board.getTotalRowsCleared();
    
    // Run the command and apply effects
    applyEffectsBefore(cmd, player);
    cmd->run(player, count, args);
    applyEffectsAfter(cmd, player);

    // Check if turn is done
    if (player.turnIsDone) {
        player.turnIsDone = false;

        // Add block to board
        player.board.addBlock(player.activeBlock);
        player.activeBlock = move(player.nextBlock);
        player.nextBlock = player.blockFactory->next();
        if (player.board.causesCollision(player.activeBlock)) {
            player.lost = true;
        }

        // Trigger special action
        int rowsCleared = player.board.getTotalRowsCleared();
        if (rowsCleared - oldRowsCleared >= Game::ClearsToTriggerAction) {
            getSpecialAction = true;
        }

        // Handle status effect
        applyEffectsAfterTurn(player);
        removeFinishedEffects(player);

        // Increment turn
        ++player.turn;
        // Skip to the next player that hasn't lost, at least one skip
        do {
            currentPlayerIdx = (currentPlayerIdx + 1) % players.size();
        }
        while(players.at(currentPlayerIdx).lost);

        // Apply status effects before turn for next player
        applyEffectsBeforeTurn(players[currentPlayerIdx]);
    }

    if (notify) {
        notifyObservers();
    }

    // Output message if special action should be performed
    if (getSpecialAction) {
        cout << "Multiline clear!" << endl;
        cout << "Enter a Special Action:" << endl;
        outputActionDescriptions();
    }
}

// Runs a special action
void Game::runAction(
        string const& cmdstr, 
        int count, 
        vector<string> args, 
        bool notify) 
{
    if (actions.find(cmdstr) == actions.end()) {
        // Action not found
        throw InvalidAction{};
    }
    
    // Current player is the affected player
    Player& affectedPlayer = players.at(currentPlayerIdx);
    
    // Original player is the closest alive player in backward direction
    int originalPlayerIdx = currentPlayerIdx;
    do {
        --originalPlayerIdx;
        if (originalPlayerIdx < 0) { originalPlayerIdx += players.size(); }
    }
    while(players.at(originalPlayerIdx).lost);
    Player& originalPlayer = players.at(originalPlayerIdx);

    // Run Special Action
    actions[cmdstr]->run(originalPlayer, affectedPlayer, args);
    
    getSpecialAction = false; //special action has been retrieved
    if (notify) {
        notifyObservers();
    }
}

void Game::outputActionDescriptions() {
    for (auto const& it : actions) {
        cout << "  " << it.first << ": " << it.second->getDesc() << endl;
    }
}

void Game::restart() {
    getSpecialAction = false;
    currentPlayerIdx = 0;
    newHighScore = false;
    tiedHighScore = false;
    initPlayers();
    notifyObservers();
}

int Game::getHighScore() const {
    return highScore;
}

// Return true if there is only one player left
// If the game is over, then 'winner' is set to the id of the remaining 
//  player
bool Game::gameIsOver() const {
    int count = 0;
    for (auto& player: players) {
        if (!player.lost) { 
            ++count;
        }
    }

    // Game is over when there is only one player left
    return count == 1;
}

vector<Player> const & Game::getPlayers() const {
    return players;
}

Player const & Game::getCurrentPlayer() const {
    return players.at(currentPlayerIdx);
}

// Add an action to the game
// A player can trigger an action if they clear enough blocks in a turn
void Game::registerAction(string name, unique_ptr<SpecialAction>&& action) {
    actions.emplace(name, move(action));
}

// Add a command to the game
void Game::registerCommand(string name, unique_ptr<Command>&& command) {
    commands.emplace(name, move(command));
}

void Game::applyEffectsBefore(CommandPtr& cmd, Player& player) {
    for (auto & e : player.statusEffects) {
        e->beforeCommand(cmd, player);
    }
}

void Game::applyEffectsAfter(CommandPtr& cmd, Player& player) {
    for (auto & e : player.statusEffects) {
        e->afterCommand(cmd, player);
    }
}

void Game::applyEffectsBeforeTurn(Player& player) {
    for (auto & e : player.statusEffects) {
        e->beforeTurn(player);
    }
}

void Game::applyEffectsAfterTurn(Player& player) {
    for (auto & e : player.statusEffects) {
        e->afterTurn(player);
    }
}

void Game::removeFinishedEffects(Player& player) {
    for (unsigned i = 0; i < player.statusEffects.size(); ++i) {
        if (player.statusEffects[i]->finished()) {
            player.statusEffects.erase(player.statusEffects.begin() + i);
        }
    }
}
