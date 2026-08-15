#pragma once
#include <string>
#include <map>

class SymbolTable {
private:
    std::map<std::string, std::string> symbols;

public:
    SymbolTable();
    void define(std::string name, std::string type);
    std::string lookup(std::string name);
    bool isDefined(std::string name);
};
