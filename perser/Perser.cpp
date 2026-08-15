#include "Parser.h"
#include 

Parser::Parser(std::vector tokens) : tokens(std::move(tokens)), pos(0) {}

const Token& Parser::current() const { return tokens[pos]; }

const Token& Parser::peekAt(size_t offset) const {
    size_t idx = pos + offset;
    if (idx >= tokens.size()) return tokens.back();
    return tokens[idx];
}

const Token& Parser::advance() {
    const Token& tok = current();
    if (!isAtEnd()) pos++;
    return tok;
}

bool Parser::isAtEnd() const { return current().kind == TokenKind::EOF_TOKEN; }

bool Parser::check(TokenKind kind) const { return current().kind == kind; }

bool Parser::match(TokenKind kind) {
    if (check(kind)) {
        advance();
        return true;
    }
    return false;
}

void Parser::skipNewlines() {
    while (check(TokenKind::NEWLINE)) advance();
}

std::vector Parser::parseProgram() {
    std::vector statements;
    skipNewlines();
    while (!isAtEnd()) {
        StmtPtr stmt = parseStatement();
        if (stmt) statements.push_back(std::move(stmt));
        skipNewlines();
    }
    return statements;
}

StmtPtr Parser::parseStatement() {
    if (check(TokenKind::PRINT)) return parsePrint();
    if (check(TokenKind::IDENT) && peekAt(1).kind == TokenKind::EQUALS) return parseAssignment();
    
    ExprPtr expr = parseExpr();
    return std::make_unique(std::move(expr));
}

StmtPtr Parser::parseAssignment() {
    std::string name = advance().lexeme;
    advance(); // Consume '='
    ExprPtr value = parseExpr();
    return std::make_unique(name, std::move(value));
}

StmtPtr Parser::parsePrint() {
    advance(); // Consume PRINT
    ExprPtr value = parseExpr();
    return std::make_unique(std::move(value));
}

ExprPtr Parser::parseExpr() {
    ExprPtr left = parseTerm();
    while (check(TokenKind::PLUS) || check(TokenKind::MINUS)) {
        std::string op = advance().lexeme;
        ExprPtr right = parseTerm();
        left = std::make_unique(op, std::move(left), std::move(right));
    }
    return left;
}

ExprPtr Parser::parseTerm() {
    ExprPtr left = parseFactor();
    while (check(TokenKind::STAR) || check(TokenKind::SLASH)) {
        std::string op = advance().lexeme;
        ExprPtr right = parseFactor();
        left = std::make_unique(op, std::move(left), std::move(right));
    }
    return left;
}

ExprPtr Parser::parseFactor() {
    if (check(TokenKind::NUMBER)) return std::make_unique(advance().lexeme);
    if (check(TokenKind::IDENT)) return std::make_unique(advance().lexeme);
    if (match(TokenKind::LPAREN)) {
        ExprPtr expr = parseExpr();
        if (check(TokenKind::RPAREN)) advance(); // Basic expect for now
        return expr;
    }
    advance(); // Skip bad token for now
    return std::make_unique("0");
}

namespace {
void printIndent(int indent) { for (int i = 0; i < indent; i++) std::cout << "  "; }

void printExpr(const Expr* expr, int indent) {
    if (!expr) return;
    if (const auto* n = dynamic_cast(expr)) {
        printIndent(indent); std::cout << "Number(" << n->value << ")\n";
    } else if (const auto* v = dynamic_cast(expr)) {
        printIndent(indent); std::cout << "Variable(" << v->name << ")\n";
    } else if (const auto* b = dynamic_cast(expr)) {
        printIndent(indent); std::cout << "Binary(" << b->op << ")\n";
        printExpr(b->left.get(), indent + 1);
        printExpr(b->right.get(), indent + 1);
    }
}

void printStmts(const std::vector& stmts, int indent);

void printStmt(const Stmt* stmt, int indent) {
    if (!stmt) return;
    if (const auto* a = dynamic_cast(stmt)) {
        printIndent(indent); std::cout << "Assign(" << a->name << ")\n";
        printExpr(a->value.get(), indent + 1);
    } else if (const auto* p = dynamic_cast(stmt)) {
        printIndent(indent); std::cout << "Print\n";
        printExpr(p->value.get(), indent + 1);
    } else if (const auto* e = dynamic_cast(stmt)) {
        printIndent(indent); std::cout << "ExprStmt\n";
        printExpr(e->expr.get(), indent + 1);
    }
}

void printStmts(const std::vector& stmts, int indent) {
    for (const auto& s : stmts) printStmt(s.get(), indent);
}
} // namespace

void printAST(const std::vector& program, int indent) {
    printStmts(program, indent);
}