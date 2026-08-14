#include "Token.h"
#include <sstream>
#include <utility>

std::string tokenKindToString(TokenKind kind) {
    switch (kind) {
        case TokenKind::NUMBER:    return "NUMBER";
        case TokenKind::IDENT:     return "IDENT";
        case TokenKind::PRINT:     return "PRINT";
        case TokenKind::NEWLINE:   return "NEWLINE";
        case TokenKind::PLUS:      return "PLUS";
        case TokenKind::MINUS:     return "MINUS";
        case TokenKind::STAR:      return "STAR";
        case TokenKind::SLASH:     return "SLASH";
        case TokenKind::EQUALS:    return "EQUALS";
        case TokenKind::LPAREN:    return "LPAREN";
        case TokenKind::RPAREN:    return "RPAREN";
        case TokenKind::EOF_TOKEN: return "EOF_TOKEN";
        case TokenKind::UNKNOWN:   return "UNKNOWN";
    }
    return "UNKNOWN";
}

Token::Token(TokenKind kind, std::string lexeme, int line)
    : kind(kind), lexeme(std::move(lexeme)), line(line) {}

std::string Token::toString() const {
    std::string displayLexeme;
    displayLexeme.reserve(lexeme.size());
    
    for (char c : lexeme) {
        if (c == '\n') {
            displayLexeme += "\\n";
        } else {
            displayLexeme += c;
        }
    }

    std::ostringstream out;
    out << "Token(" << tokenKindToString(kind) 
        << ", '" << displayLexeme 
        << "', line=" << line << ")";
        
    return out.str();
}