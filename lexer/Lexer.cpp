#include "Lexer.h"
#include <iostream>

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

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isBengaliDigitAt(const std::string& source, int pos) {
    if (pos + 2 < (int)source.length()) {
        unsigned char b1 = source[pos];
        unsigned char b2 = source[pos + 1];
        unsigned char b3 = source[pos + 2];
        return b1 == 0xE0 && b2 == 0xA7 && b3 >= 0xA6 && b3 <= 0xAF;
    }
    return false;
}

static bool isIdentStartAt(const std::string& source, int pos) {
    if (pos >= (int)source.length()) return false;
    char c = source[pos];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
        return true;
    }
    if ((unsigned char)c >= 0x80) {
        return !isBengaliDigitAt(source, pos);
    }
    return false;
}

static bool isIdentPartAt(const std::string& source, int pos) {
    if (pos >= (int)source.length()) return false;
    char c = source[pos];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || isDigit(c)) {
        return true;
    }
    if ((unsigned char)c >= 0x80) {
        return !isBengaliDigitAt(source, pos);
    }
    return false;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (pos < sourceLength) {
        skipWhitespaceAndComments();
        if (pos >= sourceLength) break;

        char ch = currentChar();

        // Newline tokenization
        if (ch == '\n') {
            tokens.push_back(Token(TokenKind::NEWLINE, "\\n", line));
            advance();
            continue;
        }

        // Single and multi-character operators
        if (ch == '+') {
            tokens.push_back(Token(TokenKind::PLUS, "+", line));
            advance();
        } else if (ch == '-') {
            tokens.push_back(Token(TokenKind::MINUS, "-", line));
            advance();
        } else if (ch == '*') {
            tokens.push_back(Token(TokenKind::STAR, "*", line));
            advance();
        } else if (ch == '/') {
            tokens.push_back(Token(TokenKind::SLASH, "/", line));
            advance();
        } else if (ch == ';') {
            tokens.push_back(Token(TokenKind::SEMICOLON, ";", line));
            advance();
        } else if (ch == '(') {
            tokens.push_back(Token(TokenKind::LPAREN, "(", line));
            advance();
        } else if (ch == ')') {
            tokens.push_back(Token(TokenKind::RPAREN, ")", line));
            advance();
        } else if (ch == '{') {
            tokens.push_back(Token(TokenKind::LBRACE, "{", line));
            advance();
        } else if (ch == '}') {
            tokens.push_back(Token(TokenKind::RBRACE, "}", line));
            advance();
        } else if (ch == '=') {
            if (peek() == '=') {
                tokens.push_back(Token(TokenKind::EQEQ, "==", line));
                advance(); // '='
                advance(); // '='
            } else {
                tokens.push_back(Token(TokenKind::EQUALS, "=", line));
                advance();
            }
        } else if (ch == '!') {
            if (peek() == '=') {
                tokens.push_back(Token(TokenKind::NE, "!=", line));
                advance();
                advance();
            } else {
                tokens.push_back(Token(TokenKind::UNKNOWN, "!", line));
                std::cout << "[Lexer Error] Unknown character '!' on line " << line << std::endl;
                advance();
            }
        } else if (ch == '<') {
            if (peek() == '=') {
                tokens.push_back(Token(TokenKind::LE, "<=", line));
                advance();
                advance();
            } else {
                tokens.push_back(Token(TokenKind::LT, "<", line));
                advance();
            }
        } else if (ch == '>') {
            if (peek() == '=') {
                tokens.push_back(Token(TokenKind::GE, ">=", line));
                advance();
                advance();
            } else {
                tokens.push_back(Token(TokenKind::GT, ">", line));
                advance();
            }
        }
        // Numbers (Arabic or Bengali)
        else if (isDigit(ch) || isBengaliDigitAt(source, pos)) {
            std::string numLex;
            while (isDigit(currentChar()) || isBengaliDigitAt(source, pos)) {
                if (isDigit(currentChar())) {
                    numLex += advance();
                } else {
                    numLex += advance(); // byte 1
                    numLex += advance(); // byte 2
                    numLex += advance(); // byte 3
                }
            }
            if (currentChar() == '.' && (isDigit(peek()) || isBengaliDigitAt(source, pos + 1))) {
                numLex += advance(); // '.'
                while (isDigit(currentChar()) || isBengaliDigitAt(source, pos)) {
                    if (isDigit(currentChar())) {
                        numLex += advance();
                    } else {
                        numLex += advance(); // byte 1
                        numLex += advance(); // byte 2
                        numLex += advance(); // byte 3
                    }
                }
            }
            tokens.push_back(Token(TokenKind::NUMBER, numLex, line));
        }
        // Identifiers and Keywords (including UTF-8 Bengali characters)
        else if (isIdentStartAt(source, pos)) {
            std::string identLex;
            while (isIdentPartAt(source, pos)) {
                identLex += advance();
            }

            // Keyword mapping
            if (identLex == "দেখাও" || identLex == "dekhaw") {
                tokens.push_back(Token(TokenKind::PRINT, identLex, line));
            } else if (identLex == "জুদি" || identLex == "zudi") {
                tokens.push_back(Token(TokenKind::IF, identLex, line));
            } else if (identLex == "নাইলে" || identLex == "naile") {
                tokens.push_back(Token(TokenKind::ELSE, identLex, line));
            } else if (identLex == "জতক্ষণ" || identLex == "zotokhon") {
                tokens.push_back(Token(TokenKind::WHILE, identLex, line));
            } else if (identLex == "পুরা" || identLex == "pura" || identLex == "ভাঙ্গা" || identLex == "bhanga") {
                tokens.push_back(Token(TokenKind::TYPE, identLex, line));
            } else {
                tokens.push_back(Token(TokenKind::IDENT, identLex, line));
            }
        }
        // Unknown characters
        else {
            std::string lex(1, ch);
            tokens.push_back(Token(TokenKind::UNKNOWN, lex, line));
            std::cout << "[Lexer Error] Unknown character '" << ch << "' on line " << line << std::endl;
            advance();
        }
    }

    tokens.push_back(Token(TokenKind::EOF_TOKEN, "", line));
    return tokens;
}
