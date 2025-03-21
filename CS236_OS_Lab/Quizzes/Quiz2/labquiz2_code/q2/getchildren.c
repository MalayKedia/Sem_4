#include "types.h"
#include "user.h"

int main(){
    for(int i = 0; i < 3; i++){
        if(fork() == 0){
            exit();
        }
    }
    getChildren();  
    for(int i = 0; i < 3; i++){
        wait();
    }
    exit();
}