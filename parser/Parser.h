#pragma once
#include <string>
#include <vector>
#include "../token/Token.h"

// AST Base classes
struct Expr {
    virtual ~Expr() = default;
};

struct NumberExpr : Expr {
    std::string value;
    NumberExpr(std::string value) {
        this->value = value;
    }
};

struct VariableExpr : Expr {
    std::string name;
    VariableExpr(std::string name) {
        this->name = name;
    }
};

struct BinaryExpr : Expr {
    std::string op;
    Expr* left;
    Expr* right;
    BinaryExpr(std::string op, Expr* left, Expr* right) {
        this->op = op;
        this->left = left;
        this->right = right;
    }
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct VarDeclStmt : Stmt {
    std::string type;
    std::string name;
    Expr* initValue; // can be nullptr
    VarDeclStmt(std::string type, std::string name, Expr* initValue) {
        this->type = type;
        this->name = name;
        this->initValue = initValue;
    }
};

struct AssignStmt : Stmt {
    std::string name;
    Expr* value;
    AssignStmt(std::string name, Expr* value) {
        this->name = name;
        this->value = value;
    }
};

struct PrintStmt : Stmt {
    Expr* value;
    PrintStmt(Expr* value) {
        this->value = value;
    }
};

struct IfStmt : Stmt {
    Expr* condition;
    std::vector<Stmt*> thenBranch;
    std::vector<Stmt*> elseBranch;
    IfStmt(Expr* condition, std::vector<Stmt*> thenBranch, std::vector<Stmt*> elseBranch) {
        this->condition = condition;
        this->thenBranch = thenBranch;
        this->elseBranch = elseBranch;
    }
};

struct WhileStmt : Stmt {
    Expr* condition;
    std::vector<Stmt*> body;
    WhileStmt(Expr* condition, std::vector<Stmt*> body) {
        this->condition = condition;
        this->body = body;
    }
};

struct ExprStmt : Stmt {
    Expr* expr;
    ExprStmt(Expr* expr) {
        this->expr = expr;
    }
};

class Parser {
public:
    Parser(std::vector<Token> tokens);

    std::vector<Stmt*> parseProgram();

    bool hasError() const { return hadError; }

private:
    std::vector<Token> tokens;
    int pos;
    bool hadError = false;

    const Token& current() const;
    const Token& peekAt(int offset) const;
    const Token& advance();
    bool isAtEnd() const;
    bool check(TokenKind kind) const;
    bool match(TokenKind kind);
    const Token& expect(TokenKind kind, const std::string& message);
    void skipNewlines();

    void reportError(const std::string& message);
    void synchronize();

    Stmt* parseStatement();
    Stmt* parseDeclaration();
    Stmt* parseAssignment();
    Stmt* parsePrint();
    Stmt* parseIf();
    Stmt* parseWhile();
    std::vector<Stmt*> parseBlock();

    // ---- expressions ----
    Expr* parseExpr();
    Expr* parseComparison();
    Expr* parseAdditive();
    Expr* parseTerm();
    Expr* parseFactor();
};

// ==================== Debug helper ====================
void printAST(const std::vector<Stmt*>& program, int indent = 0);