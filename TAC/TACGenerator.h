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

    std::string newTemp();

public:
    TACGenerator();
    ~TACGenerator();

    std::string newLabel();
    void emit(TACInstr* instr);

    std::string generateExpr(Expr* expr);
    void generateStmt(Stmt* stmt);
    std::vector<TACInstr*> generate(const std::vector<Stmt*>& program);

    const std::vector<TACInstr*>& getInstructions() const;
};
