// tac.h
#ifndef TAC_H
#define TAC_H

#include <string>
using namespace std;

// Base struct for all Three-Address Code instructions
struct TACInstr {
    virtual ~TACInstr() {}
};

// t1 = left op right   (e.g. t1 = a + b)
struct TACBinOp : public TACInstr {
    string dest;   // destination temp name, e.g. "t1"
    string left;   // left operand
    string op;     // operator string, e.g. "+", "-", "*", "/", "<", "=="
    string right;  // right operand

    TACBinOp(string d, string l, string o, string r) {
        dest = d;
        left = l;
        op = o;
        right = r;
    }
};

// dest = src   (constants and copies, e.g. x = 5 or x = t1)
struct TACCopy : public TACInstr {
    string dest;   // destination name
    string src;    // source operand (constant, variable or temp)

    TACCopy(string d, string s) {
        dest = d;
        src = s;
    }
};

// print operand
struct TACPrint : public TACInstr {
    string operand;   // what to print

    TACPrint(string o) {
        operand = o;
    }
};

// L1:
struct TACLabel : public TACInstr {
    string name;   // label name, e.g. "L1"

    TACLabel(string n) {
        name = n;
    }
};

/ goto L1
struct TACJump : public TACInstr {
    string target;   // destination label

    TACJump(string t) {
        target = t;
    }
};

// ifFalse cond goto L1
struct TACJumpIf : public TACInstr {
    string condition;   // condition variable/temp
    string target;      // destination label

    TACJumpIf(string c, string t) {
        condition = c;
        target = t;
    }
};

#endif