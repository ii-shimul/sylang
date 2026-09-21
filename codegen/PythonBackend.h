#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "../parser/Parser.h"
#include "../TAC/TACInstr.h"

class PythonBackend {
private:
    std::vector<std::string> outputLines;
    int indentLevel;

    void emit(const std::string& line);
    std::string getIndent() const;

    void generateStmt(Stmt* stmt);
    std::string generateExpr(Expr* expr);

public:
    PythonBackend();

    static std::string convertBanglaDigits(const std::string& input);

    std::string generate(const std::vector<Stmt*>& program);
    std::string generateFromTAC(const std::vector<TACInstr*>& instructions);

    std::string getCode() const;
    bool writeToFile(const std::string& filename) const;
    void print(std::ostream& out = std::cout) const;
};
