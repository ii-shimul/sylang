#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../token/Token.h"

struct Expr {
    virtual ~Expr() = default;
};
using ExprPtr = std::unique_ptr<Expr>;

struct NumberExpr : Expr {
    std::string value;
    explicit NumberExpr(std::string value) : value(std::move(value)) {}
};

struct VariableExpr : Expr {
    std::string name;
    explicit VariableExpr(std::string name) : name(std::move(name)) {}
};

struct BinaryExpr : Expr {
    std::string op;
    ExprPtr left;
    ExprPtr right;
    BinaryExpr(std::string op, ExprPtr left, ExprPtr right)
        : op(std::move(op)), left(std::move(left)), right(std::move(right)) {}
};

struct Stmt {
    virtual ~Stmt() = default;
};
using StmtPtr = std::unique_ptr<Stmt>;

struct AssignStmt : Stmt {
    std::string name;
    ExprPtr value;
    AssignStmt(std::string name, ExprPtr value)
        : name(std::move(name)), value(std::move(value)) {}
};

struct PrintStmt : Stmt {
    ExprPtr value;
    explicit PrintStmt(ExprPtr value) : value(std::move(value)) {}
};

struct IfStmt : Stmt {
    ExprPtr condition;
    std::vector<StmtPtr> thenBranch;
    std::vector<StmtPtr> elseBranch;
    IfStmt(ExprPtr condition, std::vector<StmtPtr> thenBranch, std::vector<StmtPtr> elseBranch)
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}
};

struct WhileStmt : Stmt {
    ExprPtr condition;
    std::vector<StmtPtr> body;
    WhileStmt(ExprPtr condition, std::vector<StmtPtr> body)
        : condition(std::move(condition)), body(std::move(body)) {}
};

struct ExprStmt : Stmt {
    ExprPtr expr;
    explicit ExprStmt(ExprPtr expr) : expr(std::move(expr)) {}
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    std::vector<StmtPtr> parseProgram();

    bool hasError() const { return hadError; }

private:
    std::vector<Token> tokens;
    size_t pos;
    bool hadError = false;

    const Token& current() const;
    const Token& peekAt(size_t offset) const;
    const Token& advance();
    bool isAtEnd() const;
    bool check(TokenKind kind) const;
    bool match(TokenKind kind);
    const Token& expect(TokenKind kind, const std::string& message);
    void skipNewlines();

    void reportError(const std::string& message);
    void synchronize();

    StmtPtr parseStatement();
    StmtPtr parseAssignment();
    StmtPtr parsePrint();
    StmtPtr parseIf();
    StmtPtr parseWhile();
    std::vector<StmtPtr> parseBlock();

    // ---- expressions (precedence climbing) ----
    ExprPtr parseExpr();
    ExprPtr parseTerm();
    ExprPtr parseFactor();
};

// ==================== Debug helper ====================
void printAST(const std::vector<StmtPtr>& program, int indent = 0);