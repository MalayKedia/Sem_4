/*
Testcase Description:

Same as test-fork2 but now to show that children created in the signal handler function will
behave identically to those created in normal code execution.

*/
#include "user.h"

int x;

void exitOnSignal() {
    printf(1,"I am %d. I am now exitting\n",getpid());
    exit();
}

void handler(){
    
    x++;
    int ret = fork();
    if (ret == 0) {
        printf(1 , "Hey, I exist!\n");
        signal(exitOnSignal);
        while (1) {;}
    }
    else {
        x = ret;
        sigreturn();
    }

}

int main(){
    x = 0;
    signal(handler);
    sigsend(getpid());
    sleep(1);
    printf(1,"I am %d, I have a misbehaving child %d\n",getpid(),x);
    
    int v = fork();
    if (v == 0) {
        sigsend(x);
    }
    else {
        wait();
        wait();
    }

    exit();
}