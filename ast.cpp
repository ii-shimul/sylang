#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

// ================================================================
// Phase 2: AST Base & Expression Nodes
// ================================================================

// Base class for every AST node
class ASTNode {
public:
    virtual ~ASTNode() {}
};

// NumberNode: holds numeric value
class NumberNode : public ASTNode {
public:
    double value;

    NumberNode(double value) {
        this->value = value;
    }
};

// VarNode: holds variable name
class VarNode : public ASTNode {
public:
    string name;

    VarNode(string name) {
        this->name = name;
    }
};

// BinOpNode: holds left child, right child, and operator
class BinOpNode : public ASTNode {
public:
    shared_ptr<ASTNode> left;
    shared_ptr<ASTNode> right;
    string op;

    BinOpNode(
        shared_ptr<ASTNode> left,
        string op,
        shared_ptr<ASTNode> right
    ) {
        this->left = left;
        this->op = op;
        this->right = right;
    }
};
// ================================================================
// Phase 3: AST Statement & Control Flow Nodes
// ================================================================

// AssignNode: variable name and right-side expression
class AssignNode : public ASTNode {
public:
    string variable;
    shared_ptr<ASTNode> expression;

    AssignNode(
        string variable,
        shared_ptr<ASTNode> expression
    ) {
        this->variable = variable;
        this->expression = expression;
    }
};

// PrintNode: expression to be printed
class PrintNode : public ASTNode {
public:
    shared_ptr<ASTNode> expression;

    PrintNode(shared_ptr<ASTNode> expression) {
        this->expression = expression;
    }
};

// IfNode: condition, then statements, else statements
class IfNode : public ASTNode {
public:
    shared_ptr<ASTNode> condition;
    vector<shared_ptr<ASTNode>> thenStatements;
    vector<shared_ptr<ASTNode>> elseStatements;

    IfNode(
        shared_ptr<ASTNode> condition,
        vector<shared_ptr<ASTNode>> thenStatements,
        vector<shared_ptr<ASTNode>> elseStatements
    ) {
        this->condition = condition;
        this->thenStatements = thenStatements;
        this->elseStatements = elseStatements;
    }
};

// WhileNode: condition and loop body statements
class WhileNode : public ASTNode {
public:
    shared_ptr<ASTNode> condition;
    vector<shared_ptr<ASTNode>> body;

    WhileNode(
        shared_ptr<ASTNode> condition,
        vector<shared_ptr<ASTNode>> body
    ) {
        this->condition = condition;
        this->body = body;
    }
};



