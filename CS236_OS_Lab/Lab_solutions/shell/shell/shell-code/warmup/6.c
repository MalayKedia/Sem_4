#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int N;
    printf("Enter the number of fork() calls: "); // Taking the number of for loop iterations as input
    scanf("%d", &N);

    int x = 0; // This variable is used to find the number of children we want to reap for every process

    pid_t root_id = getpid(); // The pid of the topmost parent

    for (int i = 0 ; i < N ; i ++) {
        pid_t pid = fork(); // Creating a child process
        if (pid == 0) {
            // The child created just now
            printf("Child process with PID %d\n", getpid());
            x = i + 1; // Will help determine, the number of wait statements needed
        }
    }

    // Now the number of "direct" child processes is equal to N-x
    for (int i = x; i < N; i++) {
        int r = wait(NULL);  // Reap the child process
        printf("Child process reaped with PID %d\n", r);
    }

    if (getpid() == root_id) {
        printf("All children reaped (my pid: %d)!!!\n", getpid());
    }
    return 0;
}
