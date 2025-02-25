#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    // char* args[] = {"ls", NULL}; // The list of strings to be passed to the exec system call
	char* args[] = {"ls", "-l", NULL}; // Along with another argument
	execvp(args[0], args); // See the syntax in man pages
	printf("AH! HOW DID THIS HAPPEN??\n"); // Using exec, the current process is replaced by an entirely different memory image, however if the exec call fails (which shouldn't be the case for the arguments which we have provided), this statement will get printed...
}