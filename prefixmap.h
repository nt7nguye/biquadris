#ifndef PREFIX_MAP_H
#define PREFIX_MAP_H

#include <vector>
#include <string>
#include <map>
#include <utility>

class PrefixNotFound{};
class AmbiguousPrefix{};

// Responsible for mapping a single string to a list of strings
// You can retrieve elements from this map by prefix
// If the prefix is unique, then the uniquely matching element
//  is obtatined.
class PrefixMap {
    std::map<std::string, std::vector<std::string>> commandPrefixMap;

    public:
    void add(std::string key, std::vector<std::string> values);
    void remove(std::string key);
    void rename(std::string oldKey, std::string newKey);
    bool keyExists(std::string key);
    std::pair<std::string, std::vector<std::string>> get(std::string prefix);
};

#endif
