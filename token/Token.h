#pragma once
#include <string>

enum class TokenKind {
    NUMBER, IDENT,
    PLUS, MINUS, STAR, SLASH, EQUALS,
    LPAREN, RPAREN,
    IF, ELSE, WHILE, PRINT,
    NEWLINE, EOF_TOKEN
};

std::string tokenKindToString(TokenKind kind);

class Token {
public:
    Token(TokenKind kind, std::string lexeme, int line);

    TokenKind kind;
    std::string lexeme;
    int line;
};

std::string tokenToString(const Token& token);
