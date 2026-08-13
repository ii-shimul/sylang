#include "Token.h"
#include <sstream>
#include <utility>

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
    kind(kind), lexeme(std::move(lexeme)), line(line) {}

std::string tokenToString(const Token& token){
    std::ostringstream out;
    out<< "Token(" <<tokenKindToString(token.kind)
        <<", \""<< token.lexeme << "\", line=" << token.line << ")";
    return out.str();
}