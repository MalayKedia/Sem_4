/*
Testcase Description:

Same as test-simple.c but now there is a printf statement in the child. 
This statement should not get printed as printf will not work in kernel mode

*/
#include "user.h"

int x;

void handler(){
    x++;
    printf(1,"This should not get printed!\n");
}

int main(){
    x = 0;
    signal(handler);
    sigsend(getpid());
    printf(1, "x: %d\n", x);
    exit();
}