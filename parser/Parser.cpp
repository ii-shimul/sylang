#include "Parser.h"
#include <iostream>

using namespace std;

Parser::Parser(vector<Token> tokens) : tokens(tokens), pos(0) {}

const Token& Parser::current() const {
    return tokens[pos];
}

const Token& Parser::peekAt(int offset) const {
    int idx = pos + offset;
    if (idx >= (int)tokens.size()) return tokens.back();
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

const Token& Parser::expect(TokenKind kind, const string& message) {
    if (check(kind)) {
        return advance();
    }
    reportError(message + " (got " + tokenKindToString(current().kind) + " '" + current().lexeme + "')");
    return current();
}

void Parser::skipNewlines() {
    while (check(TokenKind::NEWLINE)) advance();
}

void Parser::reportError(const string& message) {
    hadError = true;
    cout << "[Parser Error] line " << current().line << ": " << message << endl;
}

void Parser::synchronize() {
    while (!isAtEnd() && !check(TokenKind::NEWLINE) && !check(TokenKind::SEMICOLON) && !check(TokenKind::RBRACE)) {
        advance();
    }
    if (check(TokenKind::SEMICOLON) || check(TokenKind::NEWLINE)) {
        advance();
    }
}

vector<Stmt*> Parser::parseProgram() {
    vector<Stmt*> statements;
    skipNewlines();
    while (!isAtEnd()) {
        Stmt* stmt = parseStatement();
        if (stmt != nullptr) {
            statements.push_back(stmt);
        }
        skipNewlines();
    }
    return statements;
}

Stmt* Parser::parseStatement() {
    skipNewlines();
    if (check(TokenKind::TYPE)) {
        return parseDeclaration();
    }
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

    Expr* expr = parseExpr();
    if (!check(TokenKind::SEMICOLON) && !check(TokenKind::NEWLINE) && !isAtEnd() && !check(TokenKind::RBRACE)) {
        reportError("expected ';' or newline after expression");
        synchronize();
        return nullptr;
    }
    if (check(TokenKind::SEMICOLON)) {
        advance();
    }
    return new ExprStmt(expr);
}

Stmt* Parser::parseDeclaration() {
    string type = advance().lexeme; // "পুরা" or "ভাঙ্গা"
    string name = expect(TokenKind::IDENT, "expected variable name").lexeme;
    
    Expr* initVal = nullptr;
    if (match(TokenKind::EQUALS)) {
        initVal = parseExpr();
    }
    
    expect(TokenKind::SEMICOLON, "expected ';' after variable declaration");
    return new VarDeclStmt(type, name, initVal);
}

Stmt* Parser::parseAssignment() {
    string name = advance().lexeme; // identifier
    expect(TokenKind::EQUALS, "expected '=' in assignment");
    Expr* value = parseExpr();
    
    expect(TokenKind::SEMICOLON, "expected ';' after assignment");
    return new AssignStmt(name, value);
}

Stmt* Parser::parsePrint() {
    advance(); // print keyword
    expect(TokenKind::LPAREN, "expected '(' after দেখাও");
    Expr* value = parseExpr();
    expect(TokenKind::RPAREN, "expected ')' after print expression");
    expect(TokenKind::SEMICOLON, "expected ';' after print statement");
    return new PrintStmt(value);
}

Stmt* Parser::parseIf() {
    advance(); // if keyword
    expect(TokenKind::LPAREN, "expected '(' after condition");
    Expr* condition = parseExpr();
    expect(TokenKind::RPAREN, "expected ')' after condition");
    
    vector<Stmt*> thenBranch = parseBlock();
    vector<Stmt*> elseBranch;
    
    skipNewlines();
    if (check(TokenKind::ELSE)) {
        advance(); // else keyword
        elseBranch = parseBlock();
    }
    return new IfStmt(condition, thenBranch, elseBranch);
}

Stmt* Parser::parseWhile() {
    advance(); // while keyword
    expect(TokenKind::LPAREN, "expected '(' after condition");
    Expr* condition = parseExpr();
    expect(TokenKind::RPAREN, "expected ')' after condition");
    
    vector<Stmt*> body = parseBlock();
    return new WhileStmt(condition, body);
}

vector<Stmt*> Parser::parseBlock() {
    vector<Stmt*> statements;
    skipNewlines();
    if (match(TokenKind::LBRACE)) {
        skipNewlines();
        while (!check(TokenKind::RBRACE) && !isAtEnd()) {
            Stmt* stmt = parseStatement();
            if (stmt != nullptr) {
                statements.push_back(stmt);
            }
            skipNewlines();
        }
        expect(TokenKind::RBRACE, "expected '}' to close block");
        return statements;
    }
    
    Stmt* stmt = parseStatement();
    if (stmt != nullptr) {
        statements.push_back(stmt);
    }
    return statements;
}

Expr* Parser::parseExpr() {
    return parseComparison();
}

Expr* Parser::parseComparison() {
    Expr* left = parseAdditive();
    while (check(TokenKind::EQEQ) || check(TokenKind::NE) ||
           check(TokenKind::LT) || check(TokenKind::GT) ||
           check(TokenKind::LE) || check(TokenKind::GE)) {
        string op = advance().lexeme;
        Expr* right = parseAdditive();
        left = new BinaryExpr(op, left, right);
    }
    return left;
}

Expr* Parser::parseAdditive() {
    Expr* left = parseTerm();
    while (check(TokenKind::PLUS) || check(TokenKind::MINUS)) {
        string op = advance().lexeme;
        Expr* right = parseTerm();
        left = new BinaryExpr(op, left, right);
    }
    return left;
}

Expr* Parser::parseTerm() {
    Expr* left = parseFactor();
    while (check(TokenKind::STAR) || check(TokenKind::SLASH)) {
        string op = advance().lexeme;
        Expr* right = parseFactor();
        left = new BinaryExpr(op, left, right);
    }
    return left;
}

Expr* Parser::parseFactor() {
    if (check(TokenKind::NUMBER)) {
        return new NumberExpr(advance().lexeme);
    }
    if (check(TokenKind::IDENT)) {
        return new VariableExpr(advance().lexeme);
    }
    if (match(TokenKind::LPAREN)) {
        Expr* expr = parseExpr();
        expect(TokenKind::RPAREN, "expected ')' after expression");
        return expr;
    }
    
    reportError("expected number, identifier, or '(' in expression");
    advance();
    return new NumberExpr("0");
}

namespace {

void printIndent(int indent) {
    for (int i = 0; i < indent; i++) cout << "  ";
}

void printExpr(const Expr* expr, int indent) {
    if (expr == nullptr) {
        printIndent(indent);
        cout << "<null-expr>\n";
        return;
    }
    if (const auto* n = dynamic_cast<const NumberExpr*>(expr)) {
        printIndent(indent);
        cout << "Number(" << n->value << ")\n";
    } else if (const auto* v = dynamic_cast<const VariableExpr*>(expr)) {
        printIndent(indent);
        cout << "Variable(" << v->name << ")\n";
    } else if (const auto* b = dynamic_cast<const BinaryExpr*>(expr)) {
        printIndent(indent);
        cout << "Binary(" << b->op << ")\n";
        printExpr(b->left, indent + 1);
        printExpr(b->right, indent + 1);
    } else {
        printIndent(indent);
        cout << "<unknown-expr>\n";
    }
}

void printStmts(const vector<Stmt*>& stmts, int indent);

void printStmt(const Stmt* stmt, int indent) {
    if (stmt == nullptr) {
        printIndent(indent);
        cout << "<null-stmt>\n";
        return;
    }
    if (const auto* d = dynamic_cast<const VarDeclStmt*>(stmt)) {
        printIndent(indent);
        cout << "VarDecl(" << d->type << " " << d->name << ")\n";
        if (d->initValue != nullptr) {
            printExpr(d->initValue, indent + 1);
        }
    } else if (const auto* a = dynamic_cast<const AssignStmt*>(stmt)) {
        printIndent(indent);
        cout << "Assign(" << a->name << ")\n";
        printExpr(a->value, indent + 1);
    } else if (const auto* p = dynamic_cast<const PrintStmt*>(stmt)) {
        printIndent(indent);
        cout << "Print\n";
        printExpr(p->value, indent + 1);
    } else if (const auto* i = dynamic_cast<const IfStmt*>(stmt)) {
        printIndent(indent);
        cout << "If\n";
        printIndent(indent + 1);
        cout << "Condition:\n";
        printExpr(i->condition, indent + 2);
        printIndent(indent + 1);
        cout << "Then:\n";
        printStmts(i->thenBranch, indent + 2);
        if (!i->elseBranch.empty()) {
            printIndent(indent + 1);
            cout << "Else:\n";
            printStmts(i->elseBranch, indent + 2);
        }
    } else if (const auto* w = dynamic_cast<const WhileStmt*>(stmt)) {
        printIndent(indent);
        cout << "While\n";
        printIndent(indent + 1);
        cout << "Condition:\n";
        printExpr(w->condition, indent + 2);
        printIndent(indent + 1);
        cout << "Body:\n";
        printStmts(w->body, indent + 2);
    } else if (const auto* e = dynamic_cast<const ExprStmt*>(stmt)) {
        printIndent(indent);
        cout << "ExprStmt\n";
        printExpr(e->expr, indent + 1);
    } else {
        printIndent(indent);
        cout << "<unknown-stmt>\n";
    }
}

void printStmts(const vector<Stmt*>& stmts, int indent) {
    for (size_t i = 0; i < stmts.size(); i++) {
        printStmt(stmts[i], indent);
    }
}

} // namespace

void printAST(const vector<Stmt*>& program, int indent) {
    printStmts(program, indent);
}