#include "SemanticAnalyser.h"
#include <iostream>

using namespace std;

// Helper to convert Bengali/Arabic number string to double
static double parseNumber(const string& val) {
    string arabic = "";
    for (size_t i = 0; i < val.length(); i++) {
        if (i + 2 < val.length()) {
            unsigned char b1 = val[i];
            unsigned char b2 = val[i+1];
            unsigned char b3 = val[i+2];
            if (b1 == 0xE0 && b2 == 0xA7 && b3 >= 0xA6 && b3 <= 0xAF) {
                int digit = b3 - 0xA6; // 0 to 9
                arabic += to_string(digit);
                i += 2;
                continue;
            }
        }
        arabic += val[i];
    }
    try {
        return stod(arabic);
    } catch (...) {
        return 0.0;
    }
}

SemanticAnalyser::SemanticAnalyser() {}

void SemanticAnalyser::checkProgram(const vector<Stmt*>& program) {
    for (size_t i = 0; i < program.size(); i++) {
        checkStatement(program[i]);
    }
}

void SemanticAnalyser::checkStatement(Stmt* stmt) {
    if (stmt == nullptr) return;

    if (VarDeclStmt* d = dynamic_cast<VarDeclStmt*>(stmt)) {
        if (table.isDefined(d->name)) {
            errors.push_back("Error: Variable '" + d->name + "' is already defined.");
        } else {
            table.define(d->name, d->type);
        }
        if (d->initValue != nullptr) {
            string exprType = checkExpression(d->initValue);
            if (exprType != "" && exprType != d->type) {
                errors.push_back("Error: Type mismatch in declaration. Cannot initialize variable '" + d->name + "' of type '" + d->type + "' with expression of type '" + exprType + "'.");
            }
        }
    }

    else if (AssignStmt* a = dynamic_cast<AssignStmt*>(stmt)) {
        if (!table.isDefined(a->name)) {
            errors.push_back("Error: Variable '" + a->name + "' is assigned before definition.");
        } else {
            string varType = table.lookup(a->name);
            string exprType = checkExpression(a->value);
            if (exprType != "" && exprType != varType) {
                errors.push_back("Error: Type mismatch. Cannot assign type '" + exprType + "' to variable '" + a->name + "' of type '" + varType + "'.");
            }
        }
    }

    else if (PrintStmt* p = dynamic_cast<PrintStmt*>(stmt)) {
        checkExpression(p->value);
    }

    else if (IfStmt* i = dynamic_cast<IfStmt*>(stmt)) {
        checkExpression(i->condition);
        for (size_t k = 0; k < i->thenBranch.size(); k++) {
            checkStatement(i->thenBranch[k]);
        }
        for (size_t k = 0; k < i->elseBranch.size(); k++) {
            checkStatement(i->elseBranch[k]);
        }
    }

    else if (WhileStmt* w = dynamic_cast<WhileStmt*>(stmt)) {
        checkExpression(w->condition);
        for (size_t k = 0; k < w->body.size(); k++) {
            checkStatement(w->body[k]);
        }
    }

    else if (ExprStmt* e = dynamic_cast<ExprStmt*>(stmt)) {
        checkExpression(e->expr);
    }
}

string SemanticAnalyser::checkExpression(Expr* expr) {
    if (expr == nullptr) return "";

    if (NumberExpr* n = dynamic_cast<NumberExpr*>(expr)) {
        if (n->value.find('.') != string::npos) {
            return "ভাঙ্গা";
        }
        return "পুরা";
    }

    if (VariableExpr* v = dynamic_cast<VariableExpr*>(expr)) {
        if (!table.isDefined(v->name)) {
            errors.push_back("Error: Variable '" + v->name + "' is used before definition.");
            return "পুরা"; // default fallback type
        }
        return table.lookup(v->name);
    }

    if (BinaryExpr* b = dynamic_cast<BinaryExpr*>(expr)) {
        string leftType = checkExpression(b->left);
        string rightType = checkExpression(b->right);

        // Check for division by zero
        if (b->op == "/") {
            if (NumberExpr* rightNum = dynamic_cast<NumberExpr*>(b->right)) {
                if (parseNumber(rightNum->value) == 0.0) {
                    errors.push_back("Error: Division by zero is not allowed.");
                }
            }
        }

        if (leftType != "" && rightType != "" && leftType != rightType) {
            errors.push_back("Error: Type mismatch in operation '" + b->op + "'. Cannot operate on '" + leftType + "' and '" + rightType + "'.");
            return "পুরা"; // default fallback
        }
        return leftType;
    }

    return "";
}

bool SemanticAnalyser::hasError() const {
    return !errors.empty();
}

void SemanticAnalyser::report() const {
    if (errors.empty()) {
        cout << "Semantic Analysis: OK (no errors)" << endl;
    } else {
        cout << "Semantic Analysis: " << errors.size() << " error(s) found:" << endl;
        for (size_t i = 0; i < errors.size(); i++) {
            cout << "  " << errors[i] << endl;
        }
    }
}
