#include "lexer/Lexer.h"
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

    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    cout << "Tokens scanned:" << endl;
    for (size_t i = 0; i < tokens.size(); i++) {
        cout << "  " << tokens[i].toString() << endl;
    }

    return 0;
}
