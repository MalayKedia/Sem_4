/*
Testcase Description:

Idea is simple, initialise a global variable to 0
Modify the value of the global variable in the signal handler
If the signal handler is triggered, the value will be modified
Print the (hopefully) modified value

*/
#include "user.h"

int x;

void handler(){
    x++;
}

int main(){
    x = 0;
    signal(handler);
    sigsend(getpid());
    printf(1, "x: %d\n", x);
    exit();
}