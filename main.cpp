#include "lexer/Lexer.h"
#include "token/Token.h"
// TEMPORARY TAC TEST START: remove this include with the block at the end.
#include "parser/Parser.h"
#include "TAC/TACGenerator.h"
// TEMPORARY TAC TEST END
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string filename;
    cout << "Enter source file name: ";
    cin >> filename;

    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return 1;
    }

    string source = "";
    string line;
    while (getline(infile, line)) {
        source += line + "\n";
    }

    cout << "Source code:\n" << source << endl;

    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    cout << "\nTokens scanned:" << endl;
    for (size_t i = 0; i < tokens.size(); i++) {
        cout << "  " << tokens[i].toString() << endl;
    }

    return 0;
}
