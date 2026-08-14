#include "Lexer.h"

Lexer::Lexer(std::string source) : source(source), pos(0), line(1), sourceLength(source.length()) {}

char Lexer::currentChar() const {
    if (pos < sourceLength) {
        return source[pos];
    }
    return '\0';
}

char Lexer::peek() const {
    if (pos + 1 < sourceLength) {
        return source[pos + 1];
    }
    return '\0';
}

char Lexer::advance() {
    char ch = currentChar();
    if (ch != '\0') {
        pos++;
        if (ch == '\n') {
            line++;
        }
    }
    return ch;
}

void Lexer::skipWhitespaceAndComments() {
    while (pos < sourceLength) {
        char ch = currentChar();
        if (ch == ' ' || ch == '\t' || ch == '\r') {
            advance();
        } else if (ch == '/' && peek() == '/') {
            // Skip single-line comments: // ... until \n or EOF
            while (currentChar() != '\0' && currentChar() != '\n') {
                advance();
            }
        } else {
            break;
        }
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    // For Phase 1 skeleton, we just skip whitespace/comments and push EOF
    skipWhitespaceAndComments();
    
    tokens.push_back(Token(TokenKind::EOF_TOKEN, "", line));
    return tokens;
}
