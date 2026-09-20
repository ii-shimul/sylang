#include "SymbolTable.h"

using namespace std;

SymbolTable::SymbolTable() {}

void SymbolTable::define(string name, string type) {
    symbols[name] = type;
}

string SymbolTable::lookup(string name) {
    if (isDefined(name)) {
        return symbols[name];
    }
    return "";
}

bool SymbolTable::isDefined(string name) {
    return symbols.find(name) != symbols.end();
}
