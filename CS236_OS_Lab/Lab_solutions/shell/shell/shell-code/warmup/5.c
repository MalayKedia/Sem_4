#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("Before exec\n"); 
    char *args[] = {"ls", "-l", NULL}; // Example command: list files with details
    execvp(args[0], args); // Correct Command
    // args[0] = "ch"; // An invalid command
    // execvp(args[0], args);
    printf("After exec\n"); // This statement will NOT be executed, if the command is executed successfully. Uncomment the above two lines, to make this statement print
    return 0;
}