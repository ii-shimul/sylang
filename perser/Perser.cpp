#include "Parser.h"
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), pos(0) {}


const Token& Parser::current() const {
    return tokens[pos];
}

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

bool Parser::isAtEnd() const {
    return current().kind == TokenKind::EOF_TOKEN;
}

bool Parser::check(TokenKind kind) const {
    return current().kind == kind;
}

bool Parser::match(TokenKind kind) {
    if (check(kind)) {
        advance();
        return true;
    }
    return false;
}

const Token& Parser::expect(TokenKind kind, const std::string& message) {
    if (check(kind)) {
        return advance();
    }
    reportError(message + " (got " + tokenKindToString(current().kind) +
        " '" + current().lexeme + "')");
    return current();
}

void Parser::skipNewlines() {
    while (check(TokenKind::NEWLINE)) advance();
}


void Parser::reportError(const std::string& message) {
    hadError = true;
    std::cerr << "[Parser Error] line " << current().line << ": " << message << std::endl;
}

void Parser::synchronize() {
    while (!isAtEnd() && !check(TokenKind::NEWLINE)) {
        advance();
    }
    skipNewlines();
}


std::vector<StmtPtr> Parser::parseProgram() {
    std::vector<StmtPtr> statements;
    skipNewlines();
    while (!isAtEnd()) {
        StmtPtr stmt = parseStatement();
        if (stmt) {
            statements.push_back(std::move(stmt));
        }
        skipNewlines();
    }
    return statements;
}

StmtPtr Parser::parseStatement() {
    if (check(TokenKind::PRINT)) {
        return parsePrint();
    }
    if (check(TokenKind::IF)) {
        return parseIf();
    }
    if (check(TokenKind::WHILE)) {
        return parseWhile();
    }
    if (check(TokenKind::IDENT) && peekAt(1).kind == TokenKind::EQUALS) {
        return parseAssignment();
    }

    ExprPtr expr = parseExpr();
    if (!check(TokenKind::NEWLINE) && !isAtEnd() && !check(TokenKind::RBRACE)) {
        reportError("expected end of statement");
        synchronize();
        return nullptr;
    }
    return std::make_unique<ExprStmt>(std::move(expr));
}

StmtPtr Parser::parseAssignment() {
    std::string name = advance().lexeme; 
    expect(TokenKind::EQUALS, "expected '=' in assignment");
    ExprPtr value = parseExpr();

    if (!check(TokenKind::NEWLINE) && !isAtEnd() && !check(TokenKind::RBRACE)) {
        reportError("expected newline after assignment");
        synchronize();
        return nullptr;
    }
    return std::make_unique<AssignStmt>(name, std::move(value));
}

StmtPtr Parser::parsePrint() {
    advance(); 
    ExprPtr value = parseExpr();

    if (!check(TokenKind::NEWLINE) && !isAtEnd() && !check(TokenKind::RBRACE)) {
        reportError("expected newline after print statement");
        synchronize();
        return nullptr;
    }
    return std::make_unique<PrintStmt>(std::move(value));
}

StmtPtr Parser::parseIf() {
    advance(); 
    ExprPtr condition = parseExpr();
    std::vector<StmtPtr> thenBranch = parseBlock();

    std::vector<StmtPtr> elseBranch;
    skipNewlines();
    if (check(TokenKind::ELSE)) {
        advance(); 
        elseBranch = parseBlock();
    }

    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

StmtPtr Parser::parseWhile() {
    advance(); 
    ExprPtr condition = parseExpr();
    std::vector<StmtPtr> body = parseBlock();
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::vector<StmtPtr> Parser::parseBlock() {
    std::vector<StmtPtr> statements;
    skipNewlines();

    if (match(TokenKind::LBRACE)) {
        skipNewlines();
        while (!check(TokenKind::RBRACE) && !isAtEnd()) {
            StmtPtr stmt = parseStatement();
            if (stmt) statements.push_back(std::move(stmt));
            skipNewlines();
        }
        expect(TokenKind::RBRACE, "expected '}' to close block");
        return statements;
    }

    StmtPtr stmt = parseStatement();
    if (stmt) statements.push_back(std::move(stmt));
    return statements;
}


ExprPtr Parser::parseExpr() {
    ExprPtr left = parseTerm();
    while (check(TokenKind::PLUS) || check(TokenKind::MINUS)) {
        std::string op = advance().lexeme;
        ExprPtr right = parseTerm();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

ExprPtr Parser::parseTerm() {
    ExprPtr left = parseFactor();
    while (check(TokenKind::STAR) || check(TokenKind::SLASH)) {
        std::string op = advance().lexeme;
        ExprPtr right = parseFactor();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

ExprPtr Parser::parseFactor() {
    if (check(TokenKind::NUMBER)) {
        return std::make_unique<NumberExpr>(advance().lexeme);
    }
    if (check(TokenKind::IDENT)) {
        return std::make_unique<VariableExpr>(advance().lexeme);
    }
    if (match(TokenKind::LPAREN)) {
        ExprPtr expr = parseExpr();
        expect(TokenKind::RPAREN, "expected ')' after expression");
        return expr;
    }

    reportError("expected number, identifier, or '(' in expression");
    advance();
    return std::make_unique<NumberExpr>("0");
}


namespace {

void printIndent(int indent) {
    for (int i = 0; i < indent; i++) std::cout << "  ";
}

void printExpr(const Expr* expr, int indent) {
    if (!expr) {
        printIndent(indent);
        std::cout << "<null-expr>\n";
        return;
    }
    if (const auto* n = dynamic_cast<const NumberExpr*>(expr)) {
        printIndent(indent);
        std::cout << "Number(" << n->value << ")\n";
    } else if (const auto* v = dynamic_cast<const VariableExpr*>(expr)) {
        printIndent(indent);
        std::cout << "Variable(" << v->name << ")\n";
    } else if (const auto* b = dynamic_cast<const BinaryExpr*>(expr)) {
        printIndent(indent);
        std::cout << "Binary(" << b->op << ")\n";
        printExpr(b->left.get(), indent + 1);
        printExpr(b->right.get(), indent + 1);
    } else {
        printIndent(indent);
        std::cout << "<unknown-expr>\n";
    }
}

void printStmts(const std::vector<StmtPtr>& stmts, int indent);

void printStmt(const Stmt* stmt, int indent) {
    if (!stmt) {
        printIndent(indent);
        std::cout << "<null-stmt>\n";
        return;
    }
    if (const auto* a = dynamic_cast<const AssignStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "Assign(" << a->name << ")\n";
        printExpr(a->value.get(), indent + 1);
    } else if (const auto* p = dynamic_cast<const PrintStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "Print\n";
        printExpr(p->value.get(), indent + 1);
    } else if (const auto* i = dynamic_cast<const IfStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "If\n";
        printIndent(indent + 1);
        std::cout << "Condition:\n";
        printExpr(i->condition.get(), indent + 2);
        printIndent(indent + 1);
        std::cout << "Then:\n";
        printStmts(i->thenBranch, indent + 2);
        if (!i->elseBranch.empty()) {
            printIndent(indent + 1);
            std::cout << "Else:\n";
            printStmts(i->elseBranch, indent + 2);
        }
    } else if (const auto* w = dynamic_cast<const WhileStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "While\n";
        printIndent(indent + 1);
        std::cout << "Condition:\n";
        printExpr(w->condition.get(), indent + 2);
        printIndent(indent + 1);
        std::cout << "Body:\n";
        printStmts(w->body, indent + 2);
    } else if (const auto* e = dynamic_cast<const ExprStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "ExprStmt\n";
        printExpr(e->expr.get(), indent + 1);
    } else {
        printIndent(indent);
        std::cout << "<unknown-stmt>\n";
    }
}

void printStmts(const std::vector<StmtPtr>& stmts, int indent) {
    for (const auto& s : stmts) {
        printStmt(s.get(), indent);
    }
}

} 

void printAST(const std::vector<StmtPtr>& program, int indent) {
    printStmts(program, indent);
}