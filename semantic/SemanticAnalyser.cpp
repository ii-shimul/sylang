#include "SemanticAnalyser.h"
#include <iostream>

using namespace std;

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
