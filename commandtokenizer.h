#ifndef COMMAND_TOKENIZER_H
#define COMMAND_TOKENIZER_H

#include <string>
#include <vector>

// Responsible for consuming a "command string" and turning it into tokens
// A command string takes the form
//  <COUNT><COMMAND> <ARG1> <ARG2> ... <ARGN>
// Where <COUNT> is an optional integer
//       <COMMAND> is a string (no whitespace)
//       <ARG1> .. <ARGN>  are whitespace free optional strings
class CommandTokenizer {
    public:
    bool tokenize(
            std::string const & input,
            std::string& command,
            int& count, 
            std::vector<std::string>& args);
};

#endif
