#pragma once
#include <string>

// All token kinds produced by the Lexer.
enum class TokenKind {
    NUMBER,
    IDENT,
    PRINT,      // দেখাও
    NEWLINE,
    PLUS,       // +
    MINUS,      // -
    STAR,       // *
    SLASH,      // /
    EQUALS,     // =
    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }
    SEMICOLON,  // ;
    IF,         // জুদি
    ELSE,       // নাইলে
    WHILE,      // জতক্ষণ
    TYPE,       // পুরা / ভাঙ্গা
    
    // Comparison operators
    EQEQ,       // ==
    NE,         // !=
    LT,         // <
    GT,         // >
    LE,         // <=
    GE,         // >=

    EOF_TOKEN,
    UNKNOWN
};

// Human-readable name for a TokenKind (used by Token::toString())
std::string tokenKindToString(TokenKind kind);

struct Token {
    TokenKind kind;
    std::string lexeme;
    int line;

    Token(TokenKind kind, std::string lexeme, int line);

    std::string toString() const;
};