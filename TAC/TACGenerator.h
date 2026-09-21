#pragma once
#include <string>
#include <vector>
#include "TACInstr.h"
#include "../parser/Parser.h"

class TACGenerator {
private:
    std::vector<TACInstr*> instructions;
    int temp_count;
    int label_count;
    int error_count;

    std::string newTemp();
    void error(const std::string& message);
    void clear();

public:
    TACGenerator();
    ~TACGenerator();

    // The generator owns the instructions, so copying is not allowed
    TACGenerator(const TACGenerator&) = delete;
    TACGenerator& operator=(const TACGenerator&) = delete;

    std::string newLabel();
    void emit(TACInstr* instr);

    std::string generateExpr(Expr* expr);
    void generateStmt(Stmt* stmt);
    const std::vector<TACInstr*>& generate(const std::vector<Stmt*>& program);

    const std::vector<TACInstr*>& getInstructions() const;
    bool hasErrors() const;
};