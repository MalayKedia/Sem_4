#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

void sigint_handler(int sig){ // This is the signal handler, whenever SIGINT signal is pressed through keyboard, this function is called
	printf("I will run forever\n");
}

int main(){
	signal(SIGINT, sigint_handler); // This sets a mapping between the signal SIGINT and the function (it might be new to you that you can pass functions to some other functions)
	while(1); // Infinite Loop
	// kill using `kill -9 <pid>` from terminal
}