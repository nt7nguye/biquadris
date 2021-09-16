#include "commandtokenizer.h"
#include <string>
#include <sstream>

using namespace std;

// Helper for parsing the first token of input
// Given a string of the form <count><command>
//  where <count> is a natural number
//  and <command> is a string of alphabetical characters
// Sets the value of command to <command>
//  and count to <count>
void parseFirstToken(string token, string& command, int& count) {
    istringstream ss{token};
    char c;
    string countStr = "0";
    bool countIsSet = false;
    bool gettingCount = true;
    while (ss >> c) {
        if (isdigit(c) && gettingCount) {
            countStr += c;
            countIsSet = true;
        }
        else if (isalpha(c)) {
            command += c;
            gettingCount = false;
        }
    }
    count = countIsSet ? stoi(countStr) : 1;
}

// Extract command, count, and args from input
// The formate for a command is
//   <count>Command arg1 arg2 ...
// Where <count> is an optional unsigned integer, and args are optional
// 
// Return true if succesful, false otherwise
bool CommandTokenizer::tokenize(
        std::string const & input, 
        string& command, 
        int& count, 
        vector<string>& args)
{
    args.clear();
    istringstream ss{input};
    string token;
    int tokenIdx = 0;
    while(ss >> token) {
        if (tokenIdx == 0) {
            parseFirstToken(token, command, count);
        }
        else {
            args.emplace_back(token);
        }
        ++tokenIdx;
    }

    // If odx is 0, then no input was given
    return tokenIdx != 0;

}
