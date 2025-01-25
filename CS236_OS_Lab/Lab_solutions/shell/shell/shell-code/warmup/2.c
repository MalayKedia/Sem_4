#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int r = fork(); // Creating the child process
	if( r == 0 ) {
        pid_t child_pid = getpid(); // The return type of getpid() function is pid_t, however we can safely cast it to int
		printf("%d\n", child_pid); // Hope you are aware of the printf syntax
	}
	else {
		int pid = wait(NULL); // The return value of the wait() function is the child pid
		printf("%d\n", pid); // Instead of this, we can even use "r" to get the pid
	}
}