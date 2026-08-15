#include <iostream>
#include <limits>
#include <vector>
#include <string>

#include "lexer/Lexer.h"
#include "token/Token.h"

// Set UTF-8 encoding for console output (Windows support)
#ifdef _WIN32
    #include <windows.h>
    void enableUTF8Console() {
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        setvbuf(stdout, nullptr, _IOFBF, 1000);
    }
#else
    void enableUTF8Console() {
        // On Linux/Mac, UTF-8 is usually default
    }
#endif

// Function to display the language Syntax Table static
void displaySyntaxTable() {
    std::cout << "\n================ BANGLA LANGUAGE SYNTAX TABLE ================" << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
    std::cout << " Category           | Syntax / Examples       | Token Kind      " << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
    std::cout << " Identifiers        | ক, খ, ভেরিয়েবল, ক_১   | IDENT           " << std::endl;
    std::cout << " Numbers            | ০, ১, ১০, ২৫০          | NUMBER          " << std::endl;
    std::cout << " Keywords           | দেখাও                   | PRINT           " << std::endl;
    std::cout << " Assignment         | =                       | EQUALS          " << std::endl;
    std::cout << " Math Operators     | +, -, *, /              | PLUS, MINUS...  " << std::endl;
    std::cout << " Parentheses        | (, )                    | LPAREN, RPAREN  " << std::endl;
    std::cout << " Comments           | // এটি একটি মন্তব্য     | (Skipped)       " << std::endl;
    std::cout << " Line Separator     | \\n                      | NEWLINE         " << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
}

int main() {
    enableUTF8Console();  // Enable UTF-8 output for Bengali characters

    // Pure Bangla Source Code (Bangla letters + Bangla digits)
    std::string source =
        "ক = ১০\n"
        "খ = ক + ২০\n"
        "দেখাও(খ)\n"
        "দেখাও(ক)\n";

    int choice;
    
    while (true) {
        std::cout << "\n========== COMPILER MENU ==========" << std::endl;
        std::cout << "1. Lexer - Tokenize the source code" << std::endl;
        std::cout << "2. View Bangla Syntax Table" << std::endl;
        std::cout << "3. Parser - Parse tokens (Coming soon)" << std::endl;
        std::cout << "Enter your choice (1, 2, or 3): ";
        
        // Read input and validate
        if (!(std::cin >> choice)) {
            // Handle non-integer input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n[ERROR] Invalid input! Please enter 1, 2, or 3." << std::endl;
            continue;
        }
        
        std::cout << std::endl;
        
        switch (choice) {
            case 1: {
                // LEXER: Tokenize source code
                std::cout << "--- LEXER OUTPUT ---" << std::endl;
                std::cout << "Source code:" << std::endl;
                std::cout << source << std::endl;
                std::cout << "Tokens:" << std::endl;
                
                Lexer lexer(source);
                std::vector<Token> tokens = lexer.tokenize();
                
                for (const Token& tok : tokens) {
                    std::cout << tok.toString() << std::endl;
                }
                break;
            }
            case 2: {
                // SYNTAX TABLE
                displaySyntaxTable();
                break;
            }
            case 3: {
                // PARSER: To be implemented by teammate
                std::cout << "--- PARSER OUTPUT ---" << std::endl;
                std::cout << "[Coming soon] Parser will be implemented by the teammate." << std::endl;
                break;
            }
            default: {
                // ERROR: Invalid menu choice
                std::cout << "[ERROR] Invalid choice! Please enter 1, 2, or 3." << std::endl;
                continue;
            }
        }
        
        // Ask if user wants to continue
        std::cout << "\nWould you like to try again? (y/n): ";
        char again;
        if (!(std::cin >> again)) {
            break;
        }
        if (again != 'y' && again != 'Y') {
            break;
        }
    }
    
    std::cout << "\nThank you for using the Compiler!" << std::endl;
    return 0;
}