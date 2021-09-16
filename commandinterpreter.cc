#include "commandinterpreter.h"
#include <fstream>
#include <algorithm>
#include "gameexceptions.h"
#include "game.h"
#include "command.h"
#include "moveblock.h"
#include <vector>
#include <string>
#include <ctype.h>
#include <sstream>
#include <iostream>
#include <memory>
#include "commandtokenizer.h"
#include <cctype>

using namespace std;

string const CommandInterpreter::Restart = "restart";
string const CommandInterpreter::Sequence = "sequence";
string const CommandInterpreter::Rename = "rename";
string const CommandInterpreter::Macro = "macro";

string trim(const string& word) {
    const char* ws = " ";
    size_t start = word.find_first_not_of(ws);
    size_t end = word.find_last_not_of(ws);
    return start == end
        ? std::string()
        : word.substr(start, end - start + 1);
}

CommandInterpreter::CommandInterpreter(Game* game) : game{game} {
    // Register commands that get intercepted by the command interpreter
    prefixMap.add(
            CommandInterpreter::Restart,
            vector<string>{ CommandInterpreter::Restart });
    prefixMap.add(
            CommandInterpreter::Sequence,
            vector<string>{ CommandInterpreter::Sequence });
}

void CommandInterpreter::registerCommand(string const& name) {
    prefixMap.add(name, vector<string>{ name });
}

void CommandInterpreter::run(string input) {
    string commandStr = "";
    int count = 0;
    vector<string> args;

    if (tokenizer.tokenize(input, commandStr, count, args)) {
        try {
            // expand command
            string command = prefixMap.get(commandStr).second.at(0);

            if (command == CommandInterpreter::Restart) {
                game->restart();
            }
            else if (command == CommandInterpreter::Sequence) {
                if (count == 0) return;
                if (args.size() == 0) throw InvalidCommand{}; //no file 
                for (int i = 0; i < count; ++i) {
                    runSequence(args[0], input);
                }
            }
            else if (command == CommandInterpreter::Rename) {
                int numArgs = args.size();
                if (numArgs != 2) {
                    cout << "Invalid number of arguments for RENAME" << endl;
                } else {
                    string oldKey = args[0];
                    string newKey = args[1];
                    try {
                        oldKey = prefixMap.get(oldKey).first;
                        if (prefixMap.keyExists(newKey)) {
                            cout << "Cannot rename command: "
                                << "Target command already exists" << endl;
                        } else if (isdigit(newKey[0])) {
                            cout << "Cannot rename command: "
                                << "Invalid target command "
                                << "(perhaps a number is present?)" << endl;
                        } else {
                            prefixMap.rename(oldKey, newKey);
                            cout << "Command renamed!" << endl;
                        }
                    } catch (PrefixNotFound) {
                        cout << "Cannot rename command: "
                            << "Source command does not exist" << endl;
                    } catch (AmbiguousPrefix) {
                        cout << "Cannot rename command: "
                            << "Source command is ambiguous" << endl;
                    }
                }
            } else if (command == CommandInterpreter::Macro) {
                int numArgs = args.size();
                if (numArgs < 2) {
                    cout << "Invalid number of arguments for MACRO" << endl;
                } else {                    
                    string newKey = args[0];
                    if (prefixMap.keyExists(newKey)) {
                        cout << "Bad Macro name: "
                            << "Target command already exists" << endl;
                    } else if (isdigit(newKey[0])) {
                        cout << "Bad Macro name: cannot start with digit"
                            << endl;
                    } else {
                        char sepChar = ',';
                        string macroString = "";
                        for (int i = 1; i < numArgs; ++i) {
                            macroString += args[i] + " ";
                        }
                        vector<string> macroCommands;
                        stringstream ss(macroString);
                        string cmd;
                        while (getline(ss, cmd, sepChar)) {
                            macroCommands.emplace_back(trim(cmd));
                        }
                        prefixMap.add(newKey, macroCommands);
                        cout << "Macro " << newKey << " added!" << endl;
                    }
                }
            }
            else {
                // send command to the game
                vector<string> cmds = prefixMap.get(commandStr).second;
                int cmdsSize = cmds.size();
                if (cmdsSize > 1) { // macro
                    for (int i = 0; i < cmdsSize; ++i) {
                        string freshCmdStr = "";
                        bool notify = false;
                        tokenizer.tokenize(cmds[i], freshCmdStr, count, args);
                        command = prefixMap.get(freshCmdStr).second.at(0);
                        if (i == cmdsSize-1) notify = true;
                        game->run(command, count, args, notify);
                    }
                } else {
                    game->run(command, count, args, true);
                }
            }
        }
        catch(InvalidCommand) {
            cout << "Invalid Command" << endl;
        }
        catch(PrefixNotFound) {
            cout << "Command Not Found: no command matches '"
                << commandStr << "'" << endl;
        }
        catch(AmbiguousPrefix) {
            cout << "Ambiguous Prefix: multiple commands match '"
                << commandStr << "'" << endl;
        }
        catch(InvalidAction) {
            cout << "Invalid Special Action" << endl;
        }
    }
}

// Runs list of commands from a file
// The file should have one command per line
// Note: You cannot call the sequence command on a file which
//  is already being used for a sequence. If you try, the
//  command will be skipped.
void CommandInterpreter::runSequence(string filename, string input) {
    // Check if file is accessible
    ifstream file{filename};
    if (!file.good()) {
        cout << "Skipping command '" << input << "'." << endl
             << "Reason: file '"  << filename << "' does not exist." << endl;
        return;
    }
    
    // Do not run the sequence in a file if that sequence is already being run
    //  (ie. prevent this command from entering an infinite recursive loop)
    if (find(seqStack.begin(), seqStack.end(), filename) != seqStack.end()) {
        cout << "Skipping command '"
            << input
            << "'." << endl
            << "Reason: already runnning sequence file " << filename
            << endl;
        return;
    }

    seqStack.emplace_back(filename);

    string command;
    while (getline(file, command)) {
        run(command);
    }

    seqStack.pop_back();
}
