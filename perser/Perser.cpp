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