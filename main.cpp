#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lexer/Lexer.h"
#include "token/Token.h"
#include "parser/Parser.h"
#include "semantic/SemanticAnalyser.h"
#include "TAC/TACGenerator.h"
#include "codegen/PythonBackend.h"

using namespace std;

int main(int argc, char* argv[]) {
    string inputFilename;
    string outputFilename = "output.py";

    if (argc >= 2) {
        inputFilename = argv[1];
        if (argc >= 3) {
            outputFilename = argv[2];
        } else {
            size_t dotPos = inputFilename.rfind('.');
            if (dotPos != string::npos) {
                outputFilename = inputFilename.substr(0, dotPos) + ".py";
            }
        }
    } else {
        cout << "Enter source file name: ";
        if (!(cin >> inputFilename)) {
            return 1;
        }
        cout << "Enter output file name (default: output.py): ";
        string customOut;
        cin.ignore();
        if (getline(cin, customOut) && !customOut.empty()) {
            outputFilename = customOut;
        }
    }

    // 1. Read source
    ifstream infile(inputFilename);
    if (!infile.is_open()) {
        cerr << "Error: Could not open file '" << inputFilename << "'!" << endl;
        return 1;
    }

    string source = "";
    string line;
    while (getline(infile, line)) {
        source += line + "\n";
    }

    cout << "=== Compiling: " << inputFilename << " ===" << endl;

    // 2. Lexical Analysis
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();
    bool hasLexError = false;
    for (const Token& tok : tokens) {
        if (tok.kind == TokenKind::UNKNOWN) {
            hasLexError = true;
        }
    }
    if (hasLexError) {
        cerr << "Error: Compilation aborted due to lexical errors." << endl;
        return 1;
    }
    cout << "[1/4] Lexing: OK (" << tokens.size() << " tokens)" << endl;

    // 3. Parsing
    Parser parser(tokens);
    vector<Stmt*> program = parser.parseProgram();
    if (parser.hasError()) {
        cerr << "Error: Compilation aborted due to syntax errors." << endl;
        return 1;
    }
    cout << "[2/4] Parsing: OK (" << program.size() << " top-level statements)" << endl;

    // 4. Semantic Analysis
    SemanticAnalyser semantic;
    semantic.checkProgram(program);
    if (semantic.hasError()) {
        semantic.report();
        cerr << "Error: Compilation aborted due to semantic errors." << endl;
        return 1;
    }
    cout << "[3/4] Semantic Analysis: OK (no errors)" << endl;

    // 5. Intermediate Code Generation (TAC)
    TACGenerator tacGen;
    const vector<TACInstr*>& tac = tacGen.generate(program);
    if (tacGen.hasErrors()) {
        cerr << "Error: Compilation aborted due to TAC errors." << endl;
        return 1;
    }
    cout << "[4/4] TAC Generation: OK (" << tac.size() << " instructions generated)" << endl;

    // 6. Target Code Generation (Python Backend)
    PythonBackend backend;
    backend.generate(program);
    if (!backend.writeToFile(outputFilename)) {
        cerr << "Error: Could not write output file '" << outputFilename << "'!" << endl;
        return 1;
    }

    cout << "=== Compilation Successful! ===" << endl;
    cout << "Target file created: " << outputFilename << endl;

    return 0;
}
