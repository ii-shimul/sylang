#pragma once
#include <string>

// All token kinds produced by the Lexer.
enum class TokenKind {
    NUMBER,
    IDENT,
    PRINT,
    NEWLINE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EQUALS,
    LPAREN,
    RPAREN,
    EOF_TOKEN,
    UNKNOWN
};

std::string tokenKindToString(TokenKind kind);

struct Token {
    TokenKind kind;
    std::string lexeme;
    int line;

    Token(TokenKind kind, std::string lexeme, int line);

    std::string toString() const;
};