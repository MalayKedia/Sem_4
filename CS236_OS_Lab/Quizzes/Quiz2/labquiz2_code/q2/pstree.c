#include "types.h"
#include "user.h"

int main(){
    int pid = getpid();
    for(int i = 0; i < 2; i++){
        fork();
    }
    if (getpid() != pid) {sleep(20);}
    else{
        sleep (10);
        pstree();
    }
    for(int i = 0; i < 4; i++){
        wait();
    }
    exit();
}