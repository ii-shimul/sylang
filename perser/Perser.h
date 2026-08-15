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

class Parser {
public:
    explicit Parser(std::vector tokens);
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
};