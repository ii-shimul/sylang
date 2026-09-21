#include "TACGenerator.h"
#include <iostream>

using namespace std;

TACGenerator::TACGenerator() : temp_count(0), label_count(0) {}

TACGenerator::~TACGenerator() {
    for (size_t i = 0; i < instructions.size(); i++) {
        delete instructions[i];
    }
}

string TACGenerator::newTemp() {
    return "t" + to_string(temp_count++);
}

string TACGenerator::newLabel() {
    return "L" + to_string(label_count++);
}

void TACGenerator::emit(TACInstr* instr) {
    instructions.push_back(instr);
}

string TACGenerator::generateExpr(Expr* expr) {
    if (expr == nullptr) return "";

    if (NumberExpr* n = dynamic_cast<NumberExpr*>(expr)) {
        return n->value;
    }

    if (VariableExpr* v = dynamic_cast<VariableExpr*>(expr)) {
        return v->name;
    }

    if (BinaryExpr* b = dynamic_cast<BinaryExpr*>(expr)) {
        string leftName = generateExpr(b->left);
        string rightName = generateExpr(b->right);
        string dest = newTemp();
        emit(new TACBinOp(dest, leftName, b->op, rightName));
        return dest;
    }
    return "";
}

void TACGenerator::generateStmt(Stmt* stmt) {
    if (stmt == nullptr) return;

    if (VarDeclStmt* d = dynamic_cast<VarDeclStmt*>(stmt)) {
        if (d->initValue != nullptr) {
            string src = generateExpr(d->initValue);
            emit(new TACCopy(d->name, src));
        }
    }
    else if (AssignStmt* a = dynamic_cast<AssignStmt*>(stmt)) {
        string src = generateExpr(a->value);
        emit(new TACCopy(a->name, src));
    }
    else if (PrintStmt* p = dynamic_cast<PrintStmt*>(stmt)) {
        string src = generateExpr(p->value);
        emit(new TACPrint(src));
    }
    else if (ExprStmt* e = dynamic_cast<ExprStmt*>(stmt)) {
        generateExpr(e->expr);
    }
    
    //generate TAC for an if statement
    else if (IfStmt* i = dynamic_cast<IfStmt*>(stmt)) {
        string L_else = newLabel();
        string L_end = newLabel();

        string condition = generateExpr(i->condition);

        emit(new TACJumpIf(condition, L_else));

        for(Stmt* s : i->thenBranch){
            generateStmt(s);
        }

        emit(new TACJump(L_end));

        emit(new TACLabel(L_else));

        for(Stmt* s: i->elseBranch){
            generateStmt(s);
        }

        emit(new TACLabel(L_end));
    }

    else if(WhileStmt* w = dynamic_cast<WhileStmt*>(stmt)){
        string L_start = newLabel();
        string L_end = newLabel();

        emit(new TACLabel(L_start));

        string condition = generateExpr(w->condition);

        emit(new TACJumpIf(condition, L_end));

        for (Stmt* s : w->body){
            generateStmt(s);
        }

        emit(new TACJump(L_start));

        emit(new TACLabel(L_end));
    }

}

vector<TACInstr*> TACGenerator::generate(const vector<Stmt*>& program) {
    for (size_t i = 0; i < program.size(); i++) {
        generateStmt(program[i]);
    }
    return instructions;
}

const vector<TACInstr*>& TACGenerator::getInstructions() const {
    return instructions;
}
