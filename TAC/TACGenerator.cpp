#include "TACGenerator.h"
#include <iostream>

using namespace std;

TACGenerator::TACGenerator() : temp_count(0), label_count(0), error_count(0) {}

TACGenerator::~TACGenerator() {
    clear();
}

// Deletes all instructions and resets the generator to a fresh state
void TACGenerator::clear() {
    for (size_t i = 0; i < instructions.size(); i++) {
        delete instructions[i];
    }
    instructions.clear();
    temp_count = 0;
    label_count = 0;
    error_count = 0;
}

string TACGenerator::newTemp() {
    return "t" + to_string(temp_count++);
}

string TACGenerator::newLabel() {
    return "L" + to_string(label_count++);
}

void TACGenerator::error(const string& message) {
    error_count++;
    cerr << "TAC error: " << message << endl;
}

bool TACGenerator::hasErrors() const {
    return error_count > 0;
}

void TACGenerator::emit(TACInstr* instr) {
    if (instr == nullptr) return;
    instructions.push_back(instr);
}

// Returns the name (number, variable or temp) that holds the expression's value.
// Returns "" if the expression is invalid.
string TACGenerator::generateExpr(Expr* expr) {
    if (expr == nullptr) {
        error("missing expression");
        return "";
    }

    if (NumberExpr* n = dynamic_cast<NumberExpr*>(expr)) {
        if (n->value.empty()) {
            error("number literal has no value");
            return "";
        }
        return n->value;
    }

    if (VariableExpr* v = dynamic_cast<VariableExpr*>(expr)) {
        if (v->name.empty()) {
            error("variable has no name");
            return "";
        }
        return v->name;
    }

    if (BinaryExpr* b = dynamic_cast<BinaryExpr*>(expr)) {
        string leftName = generateExpr(b->left);
        string rightName = generateExpr(b->right);

        // Don't emit broken TAC if either side failed
        if (leftName.empty() || rightName.empty()) return "";

        string dest = newTemp();
        emit(new TACBinOp(dest, leftName, b->op, rightName));
        return dest;
    }
    error("unsupported expression type");
    return "";
}

void TACGenerator::generateStmt(Stmt* stmt) {
    if (stmt == nullptr) {
        error("missing statement");
        return;
    }

    if (VarDeclStmt* d = dynamic_cast<VarDeclStmt*>(stmt)) {
        // Declaration without a value generates no code
        if (d->initValue == nullptr) return;

        string src = generateExpr(d->initValue);
        if (!src.empty()) emit(new TACCopy(d->name, src));
    }
    else if (AssignStmt* a = dynamic_cast<AssignStmt*>(stmt)) {
        string src = generateExpr(a->value);
        if (!src.empty()) emit(new TACCopy(a->name, src));
    }
    else if (PrintStmt* p = dynamic_cast<PrintStmt*>(stmt)) {
        string src = generateExpr(p->value);
        if (!src.empty()) emit(new TACPrint(src));
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

    else {
        error("unsupported statement type");
    }
}

// Check hasErrors() after calling this before using the result
const vector<TACInstr*>& TACGenerator::generate(const vector<Stmt*>& program) {
    clear();
    for (size_t i = 0; i < program.size(); i++) {
        generateStmt(program[i]);
    }
    return instructions;
}

const vector<TACInstr*>& TACGenerator::getInstructions() const {
    return instructions;
}