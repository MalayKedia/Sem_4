/*
Testcase Description:

Idea is to show that syscalls will now work.
Fork, wait, printf and exit should all work.

*/
#include "user.h"

int x;

void handler(){
    
    x++;
    int ret = fork();
    if (ret == 0) {
        printf(1 , "Hey, I exist!\n");
        exit();
    }
    else {
        wait();
    }

    sigreturn();
}

int main(){
    x = 0;
    signal(handler);
    int ret = fork();
    if (ret == 0) {
        sleep(1);
        printf(1,"X: %d\n",x);
        signal(handler);
        sleep(1);
        printf(1,"x: %d\n",x);
    }
    else {
        sigsend(ret);
        sleep(2);
        sigsend(ret);
        wait();
    }
    exit();
}