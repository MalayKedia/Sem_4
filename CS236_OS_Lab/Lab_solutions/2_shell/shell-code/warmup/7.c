#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int main(){
	int r = fork();
	if ( r == 0 ) {
		sleep(10); // The child sleeps for 10 seconds, hopefully your scheduler will give a chance to the parent process in these 10 seconds
	}
	else {
		printf("Forked child %d\n", r);
		sleep(2); // The parent also sleeps for 2 seconds
		kill(r, SIGTERM); // Kill the child with this id
		int pid = wait(NULL); // Reap the child
		printf("Killed child %d\n", pid); // Proof that the code is correct: it should take less than 10 seconds to run...
	}
}