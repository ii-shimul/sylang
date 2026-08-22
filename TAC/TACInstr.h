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


// TACCopy

struct TACCopy : public TACInstr {
    std::string dest;
    std::string src;

    TACCopy(std::string dest, std::string src) {
        this->dest = dest;
        this->src = src;
    }
};

// TACPrint

struct TACPrint : public TACInstr {
    std::string operand;

    TACPrint(std::string operand) {
        this->operand = operand;
    }
};

// TACLabel

struct TACLabel : public TACInstr {
    std::string label;

    TACLabel(std::string label) {
        this->label = label;
    }
};


// TACJump

struct TACJump : public TACInstr {
    std::string target;

    TACJump(std::string target) {
        this->target = target;
    }
};


// TACJumpIf

struct TACJumpIf : public TACInstr {
    std::string cond;
    std::string target;

    TACJumpIf(std::string cond, std::string target) {
        this->cond = cond;
        this->target = target;
    }
};

#endif // TAC_INSTR_H


