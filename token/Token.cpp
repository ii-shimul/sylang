#include "Token.h"
#include <sstream>

std:: string tokenKindToString(TokenKind kind){
    switch(kind){
        case TokenKind::NUMBER: return "NUMBER";
        case TokenKind::IDENT: return "IDENT";
        case TokenKind::PLUS: return "PLUS";
        case TokenKind::MINUS: return "MINUS";
        case TokenKind::STAR: return "STAR";
        case TokenKind::SLASH: return "SLASH";
        case TokenKind::EQUALS: return "EQUALS";
        case TokenKind::LPAREN: return "LPAREN";
        case TokenKind::RPAREN: return "RPAREN";
        case TokenKind::IF: return "IF";
        case TokenKind::ELSE: return "ELSE";
        case TokenKind::WHILE: return "WHILE";
        case TokenKind::PRINT: return "PRINT";
        case TokenKind::NEWLINE: return "NEWLINE";
        case TokenKind::EOF_TOKEN: return "EOF";
    }
    return "Unknown";
}

Token::Token(TokenKind kind, std::string lexeme, int line):
    kind(kind), lexeme(lexeme), line(line) {}

std::string Token::toString() const {
    std::ostringstream out;
    out << "Token(" << tokenKindToString(kind)
        << ", \"" << lexeme << "\", line=" << line << ")";
    return out.str();
}