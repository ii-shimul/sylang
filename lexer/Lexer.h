#pragma once

#include<string>
#include<vector>
#include "../token/Token.h"

class Lexer{
public:
    explicit Lexer(std::string source);

    std::vector<Token> tokenize();

private:
    std::string source; 
    // Uses unsigned size_t to match source.size() and avoid signed/unsigned comparison warnings
    size_t pos; 
    int line;
    std::vector<Token> tokens;

    //UTF-8 helpers functions
    static int utf8CharLen(unsigned char leadByte);
    static unsigned long decodeUtf8CodePoint(const std::string& s, size_t bytePos, int len);
    static bool isBanglaDigitCP(unsigned long codePoint);
    static bool isBanglaLetterCP(unsigned long codePoint);

    //Character-stream-access
    std::string currentChar() const;
    std::string peak() const;
    std::string advance();

    // ASCII + Bangla
    bool isDigitChar(const std::string& ch) const;
    bool isAlphaChar(const std::string& ch) const;
    bool isAlnumChar(const std::string& ch) const;

    //sub lexer
    Token readNumber();
    Token readIdentOrKeyword();
};