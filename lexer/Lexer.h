#pragma once
#include "../token/Token.h"
#include <string>
#include <vector>

class Lexer {
private:
    std::string source;
    int pos;
    int line;
    int sourceLength;

    char currentChar() const;
    char peek() const;
    char advance();
    void skipWhitespaceAndComments();

public:
    Lexer(std::string source);
    std::vector<Token> tokenize();
};
