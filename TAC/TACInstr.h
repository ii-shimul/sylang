#ifndef TAC_INSTR_H
#define TAC_INSTR_H

#include <string>

// Base struct for all Three-Address Code (TAC) instructions.

struct TACInstr {
    virtual ~TACInstr() {}
};

// TACBinOp

struct TACBinOp : public TACInstr {
    std::string dest;
    std::string left;
    std::string op;
    std::string right;

    TACBinOp(std::string dest, std::string left, std::string op, std::string right) {
        this->dest = dest;
        this->left = left;
        this->op = op;
        this->right = right;
    }
};


