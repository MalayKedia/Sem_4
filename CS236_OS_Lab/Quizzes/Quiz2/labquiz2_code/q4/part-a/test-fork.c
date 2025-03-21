/*
Testcase Description:

The implementation described by us in the question means that by default the handler is NOT passed to child
Thus, the first x printed should read 0
The second x is printed after the child sets its signal handler and hence it should be 1

*/
#include "user.h"
int x;

void handler(){
    x++;
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