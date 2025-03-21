/*
Testcase Description:

Testcase checks if trapframe handling is correct. 
If it isn't, modification to ecx value in handler will be visible in code.
If it is, then trapframe is not being saved and restored correctly.  

*/
#include "user.h"

int x;

void handler(){
    x++;
    __asm__ volatile (
        "movl $0, %%ecx"  // Move immediate value 0 into ECX
        :                 // No output operands
        :                 // No input operands
        : "ecx"           // Clobber list (marks ECX as modified)
    );
    sigreturn();
}

int main(){
    x = 0;
    int val;
    __asm__ volatile (
        "movl $3, %%ecx"  // Move immediate value 3 into ECX
        :                 // No output operands
        :                 // No input operands
        : "ecx"           // Clobber list (marks ECX as modified)
    );
    signal(handler);
    sigsend(getpid());
    __asm__ volatile (
        "movl %%ecx, %0"  // Move ECX's value into 'val'
        : "=r" (val)      // Output operand
        :                 // No input operands
    );
    printf(1, "x: %d\nval: %d\n", x,val);
    exit();
}