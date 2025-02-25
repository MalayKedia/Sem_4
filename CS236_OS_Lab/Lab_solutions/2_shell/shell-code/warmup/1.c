#include <stdio.h>  // Standard Input-Output Library
#include <sys/types.h> // Used for pid_t (type of process ids), although int works, so this library not needed
#include <sys/wait.h> // For wait and waitpid functions
#include <fcntl.h> // Opening and Locking file descriptors (this too not needed in this code)
#include <unistd.h> // Functions like fork, getpid, exec, getppid are present in this library

int main(){
	int r = fork(); // Used to create a child process
	if(r == 0){ // From this line, code will be run in both parent and child processes
		printf("I am child\n"); // The return value is 0 for the child
	}
	else{ // Here the value of r (return value of fork()) is the process id of the parent process
		wait(NULL); // Wait until the child process finishes, and reaps it		
		printf("I am parent\n");
	}
	// Ideally we should have another case, where r < 0, and the fork function fails
}