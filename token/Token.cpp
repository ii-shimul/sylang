#include "Token.h"

std::string tokenKindToString(TokenKind kind) {
    switch (kind) {
        case TokenKind::NUMBER: return "NUMBER";
        case TokenKind::IDENT: return "IDENT";
        case TokenKind::PLUS: return "PLUS";
        case TokenKind::MINUS: return "MINUS";
        case TokenKind::STAR: return "STAR";
        case TokenKind::SLASH: return "SLASH";
        case TokenKind::EQUALS: return "EQUALS";
        case TokenKind::LPAREN: return "LPAREN";
        case TokenKind::RPAREN: return "RPAREN";
        case TokenKind::LBRACE: return "LBRACE";
        case TokenKind::RBRACE: return "RBRACE";
        case TokenKind::SEMICOLON: return "SEMICOLON";
        case TokenKind::IF: return "IF";
        case TokenKind::ELSE: return "ELSE";
        case TokenKind::WHILE: return "WHILE";
        case TokenKind::TYPE: return "TYPE";
        case TokenKind::PRINT: return "PRINT";
        case TokenKind::NEWLINE: return "NEWLINE";
        case TokenKind::EQEQ: return "EQEQ";
        case TokenKind::NE: return "NE";
        case TokenKind::LT: return "LT";
        case TokenKind::GT: return "GT";
        case TokenKind::LE: return "LE";
        case TokenKind::GE: return "GE";
        case TokenKind::EOF_TOKEN: return "EOF";
        case TokenKind::UNKNOWN: return "UNKNOWN";
    }
    return "Unknown";
}

Token::Token(TokenKind kind, std::string lexeme, int line):
    kind(kind), lexeme(lexeme), line(line) {}

std::string Token::toString() const {
    return "Token(" + tokenKindToString(kind) + ", \"" + lexeme + "\", line=" + std::to_string(line) + ")";
}