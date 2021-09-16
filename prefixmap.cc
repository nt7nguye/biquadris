#include "prefixmap.h"
#include <string>
#include <vector>
#include <map>
#include <utility>

using namespace std;

// Add(string, vector<string>) 
// Adds a command string to the map with its command list
void PrefixMap::add(string key, vector<string> values) {
    commandPrefixMap.emplace(key, values);
}

// Remove(string) removes the given key
void PrefixMap::remove(string key) {
    if (commandPrefixMap.find(key) != commandPrefixMap.end()) { // exists
        commandPrefixMap.erase(key);
    }
}

// Rename(string, string) 
// Assigns the value of oldKey to newKey and removes oldKey
void PrefixMap::rename(string oldKey, string newKey) {
    if (commandPrefixMap.find(oldKey) != commandPrefixMap.end()) { // exists
        commandPrefixMap[newKey] = commandPrefixMap[oldKey];
        commandPrefixMap.erase(oldKey);
    }
}

// KeyExists(string) checks if the given key exists in the map
// (just the given string, does not treat the string as a prefix)
bool PrefixMap::keyExists(string key) {
    if (commandPrefixMap.find(key) != commandPrefixMap.end()) { // exists
        return true;
    }
    return false;
}

// Get(string) tries to get the command and command list associated 
//  with the given prefix
pair<string, vector<string>> PrefixMap::get(string prefix) {
    int count = 0;
    string correctKey;

    // If any key in our map starts with our prefix,
    //  set correctKey and increment count
    for (auto pair: commandPrefixMap) {
        string key = pair.first;
        if (key == prefix) {
            // perfect match. return the command
            count = 1;
            correctKey = key;
            break;
        }
        if (key.rfind(prefix, 0) == 0) {
            correctKey = key;
            ++count;
        }
    }

    // If there is only one occurrence of the required command,
    //  then return that key
    if (count == 1) {
        return make_pair(correctKey, commandPrefixMap[correctKey]);
    }
    else if (count == 0) {
        throw PrefixNotFound{};
    }
    // ... else multiple keys matched
    throw AmbiguousPrefix{};
}
