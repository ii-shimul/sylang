#pragma once
#include 
#include 
#include 
#include "../token/Token.h" 
 

struct Expr { virtual ~Expr() = default; };
using ExprPtr = std::unique_ptr;

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

struct Stmt { virtual ~Stmt() = default; };
using StmtPtr = std::unique_ptr;

struct AssignStmt : Stmt {
    std::string name;
    ExprPtr value;
    AssignStmt(std::string name, ExprPtr value) : name(std::move(name)), value(std::move(value)) {}
};

struct PrintStmt : Stmt {
    ExprPtr value;
    explicit PrintStmt(ExprPtr value) : value(std::move(value)) {}
};

struct ExprStmt : Stmt {
    ExprPtr expr;
    explicit ExprStmt(ExprPtr expr) : expr(std::move(expr)) {}
};

class Parser {
public:
    explicit Parser(std::vector tokens);

    std::vector parseProgram();
private:
    std::vector tokens;
    size_t pos;

    const Token& current() const;
    const Token& peekAt(size_t offset) const;
    const Token& advance();
    bool isAtEnd() const;
    bool check(TokenKind kind) const;
    bool match(TokenKind kind);
    void skipNewlines();
    
    
    StmtPtr parseStatement();
    StmtPtr parseAssignment();
    StmtPtr parsePrint();

    
    // ---- expressions (precedence climbing) ----
    ExprPtr parseExpr();
    ExprPtr parseTerm();
    ExprPtr parseFactor();


};