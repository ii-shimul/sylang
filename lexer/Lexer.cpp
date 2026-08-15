#include "./Lexer.h"
#include<cctype>
#include<iostream>

Lexer::Lexer(std::string source) : source(std::move(source)), pos(0), line(1){}


//UTF-8 helper functions

//Number of bytes in the UTF-8 char
int Lexer::utf8CharLen(unsigned char leadByte){
    if((leadByte & 0x80) == 0x00) return 1; //ASCII
    if((leadByte & 0xE0) == 0xC0) return 2; 
    if((leadByte & 0xF0) == 0xE0) return 3;// Bangla
    if((leadByte & 0xF8) == 0xF0) return 4;
    return 1;
}

unsigned long Lexer::decodeUtf8CodePoint(const std::string& s, size_t bytePos, int len){
    unsigned char first = static_cast<unsigned char>(s[bytePos]);
    unsigned long cp = 0;

    switch(len){
        case 1: cp = first;         break;
        case 2: cp = first & 0x1F;  break;
        case 3: cp = first & 0x0F;  break;
        case 4: cp = first & 0x07;  break;
        default: cp = first;        break;
    };

    for (int i = 1; i < len && bytePos + i <s.size(); i++){
        unsigned char count = static_cast<unsigned char>(s[bytePos + i]);
        cp = (cp << 6) | (count & 0x3F);
    }

    return cp;
}

//Bangla digits occupy U+09E6 - U=09EF.
bool Lexer::isBanglaDigitCP(unsigned long codePoint){
    return codePoint >= 0x09E6 && codePoint <= 0x09EF;
}

//Bangla block is U+0980 - U+09FF
bool Lexer::isBanglaLetterCP(unsigned long codePoint){
    return codePoint >= 0x0980 && codePoint <= 0x09FF && !isBanglaDigitCP(codePoint);
}


// Character-stream access
std::string Lexer::currentChar() const{
    if (pos >= source.size()) return "";

    int len = utf8CharLen(static_cast<unsigned char>(source[pos]));
    if(pos + static_cast<size_t>(len) > source.size()) len = 1;
    return source.substr(pos, len);
}

std::string Lexer::peek() const {
    if (pos >= source.size()) return "";

    int len = utf8CharLen(static_cast<unsigned char>(source[pos]));
    if(pos + static_cast<size_t>(len) > source.size()) len = 1;

    size_t nextPos = pos + len;
    if(nextPos >= source.size()) return "";

    int nextLen =  utf8CharLen(static_cast<unsigned char>(source[nextPos]));

    if(nextPos + static_cast<size_t>(nextLen) > source.size()) nextLen = 1;

    return source.substr(nextPos, nextLen);
}


std::string Lexer::advance() {
    std:: string ch = currentChar();
    pos += ch.size();
    if(ch == "\n"){
        line++;
    }
    return ch;
}


// Classification
bool Lexer::isDigitChar(const std::string& ch) const {
    if (ch.empty()) return false;
    return isBanglaDigitCP(decodeUtf8CodePoint(ch, 0, static_cast<int> (ch.size())));
}

bool Lexer::isAlphaChar(const std::string& ch) const{
    if(ch.empty()) return false;
    return isBanglaLetterCP(decodeUtf8CodePoint(ch, 0, static_cast<int>(ch.size())));
}

bool Lexer::isAlnumChar(const std::string& ch) const {
    return isAlphaChar(ch) || isDigitChar(ch);
}


Token Lexer::readNumber() {
    int startLine = line;
    std::string numberStr;
    while (isDigitChar(currentChar())) {
        numberStr += advance();
    }
    return Token(TokenKind::NUMBER, numberStr, startLine);
}

Token Lexer::readIdentOrKeyword() {
    int startLine = line;
    std::string word;
    while (isAlnumChar(currentChar()) || currentChar() == "_") {
        word += advance();
    }

    // Support only Bangla PRINT keyword ("দেখাও").
    if (word == "\u09A6\u09C7\u0996\u09BE\u0993") {
        return Token(TokenKind::PRINT, word, startLine);
    }
    return Token(TokenKind::IDENT, word, startLine);
}


std::vector<Token> Lexer::tokenize() {
    while (pos < source.size()) {
        std::string ch = currentChar();

        if (ch == " " || ch == "\t") {
            advance();
        } else if (ch == "/" && peek() == "/") {
            while (currentChar() != "\n" && currentChar() != "") {
                advance();
            }
        } else if (ch == "\n") {
            tokens.emplace_back(TokenKind::NEWLINE, "\\n", line);
            advance();
        } else if (ch == "+") {
            tokens.emplace_back(TokenKind::PLUS, "+", line);
            advance();
        } else if (ch == "-") {
            tokens.emplace_back(TokenKind::MINUS, "-", line);
            advance();
        } else if (ch == "*") {
            tokens.emplace_back(TokenKind::STAR, "*", line);
            advance();
        } else if (ch == "/") {
            tokens.emplace_back(TokenKind::SLASH, "/", line);
            advance();
        } else if (ch == "=") {
            tokens.emplace_back(TokenKind::EQUALS, "=", line);
            advance();
        } else if (ch == "(") {
            tokens.emplace_back(TokenKind::LPAREN, "(", line);
            advance();
        } else if (ch == ")") {
            tokens.emplace_back(TokenKind::RPAREN, ")", line);
            advance();
        } else if (isDigitChar(ch)) {
            tokens.push_back(readNumber());
        } else if (isAlphaChar(ch) || ch == "_") {
            tokens.push_back(readIdentOrKeyword());
        } else {
            std::cerr << "[Lexer Error] Unknown character '" << ch
                      << "' on line " << line << std::endl;
            advance();
        }
    }

    if (!tokens.empty() && tokens.back().kind != TokenKind::NEWLINE) {
        tokens.emplace_back(TokenKind::NEWLINE, "\\n", line);
    }

    tokens.emplace_back(TokenKind::EOF_TOKEN, "", line);
    return tokens;
}