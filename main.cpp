#include <iostream>
#include <locale>
#include <vector>
#include "token/Token.h"

int main() {
    std::setlocale(LC_ALL, "");

    const std::vector<Token> tokens = {
        Token(TokenKind::IF, "ধরি", 1),
        Token(TokenKind::IDENT, "ক", 1),
        Token(TokenKind::EQUALS, "=", 1),
        Token(TokenKind::NUMBER, "১০", 1),
        Token(TokenKind::NEWLINE, "\n", 1),

        Token(TokenKind::IF, "ধরি", 2),
        Token(TokenKind::IDENT, "খ", 2),
        Token(TokenKind::EQUALS, "=", 2),
        Token(TokenKind::NUMBER, "২০", 2),
        Token(TokenKind::NEWLINE, "\n", 2),

        Token(TokenKind::IF, "যদি", 3),
        Token(TokenKind::LPAREN, "(", 3),
        Token(TokenKind::IDENT, "ক", 3),
        Token(TokenKind::PLUS, "+", 3),
        Token(TokenKind::IDENT, "খ", 3),
        Token(TokenKind::EQUALS, "==", 3),
        Token(TokenKind::NUMBER, "৩০", 3),
        Token(TokenKind::RPAREN, ")", 3),
        Token(TokenKind::NEWLINE, "\n", 3),

        Token(TokenKind::PRINT, "প্রিন্ট", 4),
        Token(TokenKind::IDENT, "ফলাফল ৩০", 4),
        Token(TokenKind::NEWLINE, "\n", 4),

        Token(TokenKind::EOF_TOKEN, "", 5)
    };

    std::cout << "Bangla source:\n";
    std::cout << "ধরি ক = ১০\n";
    std::cout << "ধরি খ = ২০\n";
    std::cout << "যদি (ক + খ == ৩০) {\n";
    std::cout << "    প্রিন্ট \"ফলাফল ৩০\"\n";
    std::cout << "}\n\n";

    std::cout << "Token output:\n";
    for (const Token& token : tokens) {
        std::cout << token.toString() << '\n';
    }

    std::cout << "\nBangla number status: " << banglaValueStatus("১০") << '\n';
    std::cout << "Bangla word status: " << banglaValueStatus("আমি") << '\n';
    std::cout << "Not Bangla status: " << banglaValueStatus("hello") << '\n';

    return 0;
}
