// TestTAC.cpp
#include <iostream>
#include <vector>
#include "TACInstr.h"
using namespace std;

int main() {
    vector<TACInstr*> code;

    code.push_back(new TACCopy("a", "5"));
    code.push_back(new TACBinOp("t1", "a", "+", "3"));
    code.push_back(new TACPrint("t1"));
    code.push_back(new TACLabel("L1"));
    code.push_back(new TACJumpIf("t1", "L2"));
    code.push_back(new TACJump("L1"));

    // Identify each instruction using dynamic_cast
    for (int i = 0; i < code.size(); i++) {
        if (TACBinOp* b = dynamic_cast<TACBinOp*>(code[i])) {
            cout << b->dest << " = " << b->left << " " << b->op << " " << b->right << endl;
        } else if (TACCopy* c = dynamic_cast<TACCopy*>(code[i])) {
            cout << c->dest << " = " << c->src << endl;
        } else if (TACPrint* p = dynamic_cast<TACPrint*>(code[i])) {
            cout << "print " << p->operand << endl;
        } else if (TACLabel* l = dynamic_cast<TACLabel*>(code[i])) {
            cout << l->name << ":" << endl;
        } else if (TACJump* j = dynamic_cast<TACJump*>(code[i])) {
            cout << "goto " << j->target << endl;
        } else if (TACJumpIf* ji = dynamic_cast<TACJumpIf*>(code[i])) {
            cout << "ifFalse " << ji->condition << " goto " << ji->target << endl;
        }
    }

    // Free memory (virtual destructor makes this safe)
    for (int i = 0; i < code.size(); i++) {
        delete code[i];
    }
    return 0;
}