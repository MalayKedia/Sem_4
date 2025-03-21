#include "types.h"
#include "user.h"

int main(){
    for(int i = 0; i < 3; i++){
        if(fork() == 0){
            exit();
        }
    }
    
    int child_pid = fork();
    if (child_pid == 0){
        getSibling();
        exit();
    }

    else{
        sleep(3);
        wait();
    }


    for(int i = 0; i < 3; i++){
        wait();
    }
    exit();
}