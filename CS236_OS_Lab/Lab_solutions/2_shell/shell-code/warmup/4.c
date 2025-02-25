#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) { // Please go through this syntax of int main(), might help later
    // argc denotes the number of command line arguments. So, if we run "./a.out cmd1 cmd2", argc will be equal to 3, argv denotes the corresponding arguments
    if (argc != 3) {
        printf("Incorrect number of arguments\n");
        return 1; // This return value indicates an error/abnormal termination
    }

    pid_t pid = fork(); // Forking a child process
    
    if ( pid == 0 ) {
        // We are inside child...
        if (execvp(argv[1], &argv[1]) == -1) { // argv[1] contains the command we need to execute, &argv[1] is basically the pointer pointing to argv[1]. Basically, &argv[1] is the list with 2 elements, the command and its argument
            perror("Exec failed"); // We will reach here only if the exec fails
            return 1;
        }
    } 
    else {
        // We are inside parent...
        wait(NULL); // Waiting for the child to finish
        printf("Command successfully completed\n");
    }
    return 0;
}