#ifndef COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H

#include <string>
#include "game.h"
#include "prefixmap.h"
#include "commandtokenizer.h"
#include <vector>

class CommandInterpreter {
    public:
    static std::string const Restart;
    static std::string const Sequence;
    static std::string const Rename;
    static std::string const Macro;

    private:
    Game* game;
    PrefixMap prefixMap;
    CommandTokenizer tokenizer;
    std::vector<std::string> seqStack; //list of currently open files
                                       // being used for the 'sequence' command
    void runSequence(std::string file, std::string input);

    public:
    CommandInterpreter(Game*);
    void registerCommand(std::string const&);
    void run(std::string);
};

#endif
