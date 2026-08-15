#pragma once
#include "SymbolTable.h"
#include "../parser/Parser.h"
#include <string>
#include <vector>

class SemanticAnalyser {
private:
    SymbolTable table;
    std::vector<std::string> errors;

public:
    SemanticAnalyser();
    
    void checkProgram(const std::vector<Stmt*>& program);
    void checkStatement(Stmt* stmt);
    std::string checkExpression(Expr* expr);
    
    bool hasError() const;
    void report() const;
};
