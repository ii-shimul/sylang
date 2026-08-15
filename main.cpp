#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "semantic/SemanticAnalyser.h"
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

    cout << "\nTokens scanned:" << endl;
    for (size_t i = 0; i < tokens.size(); i++) {
        cout << "  " << tokens[i].toString() << endl;
    }

    cout << "\nParsing program..." << endl;
    Parser parser(tokens);
    vector<Stmt*> program = parser.parseProgram();

    if (parser.hasError()) {
        cout << "Parsing failed due to errors." << endl;
    } else {
        cout << "Parsing successful! Abstract Syntax Tree (AST):" << endl;
        printAST(program);

        cout << "\nRunning semantic analysis..." << endl;
        SemanticAnalyser analyser;
        analyser.checkProgram(program);
        analyser.report();
    }

    return 0;
}
