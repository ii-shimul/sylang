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
