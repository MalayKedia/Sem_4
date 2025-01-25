#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

// Macros for constants
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens */
char **tokenize (char* line) {
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;
    for (i = 0 ; i < strlen(line) ; i ++) {
        char readChar = line[i];
        if (readChar == ' ' || readChar == '\n' || readChar == '\t') {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0){
                tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0; 
            }
        }
        else {
            token[tokenIndex++] = readChar;
        }
    }
    free(token);
    tokens[tokenNo] = NULL ;
    return tokens;
}

void free_mem(char **tokens){
    // To free the memory taken by these strings
	for(int i = 0 ; tokens[i] != NULL ; i++ ) {
		free(tokens[i]);
	}
	free(tokens);
}

int main(int argc, char* argv[]) {
    char line[MAX_INPUT_SIZE];
    char** tokens;
    int i;

    while (1) {
        /* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line)); // Set all elements as 0
		printf("$ "); // The initial character, which asks for user prompt
		scanf("%[^\n]", line); // Everything except "\n" is accepted
		getchar(); // For taking the "\n" from user
        line[strlen(line)] = '\n'; //terminate with new line
        tokens = tokenize(line);

        if(tokens[0] && !strcmp(tokens[0],"exit") && !tokens[1]){
			// If first token is exit, and 2nd is NULL then free memory
			free_mem(tokens);
			kill(-getpid(), SIGTERM); // Kill all processes in same pgid; see how we are using -<pid-of-main-shell> to kill all the processes from the same group
			exit(0);
		}

		while(waitpid(-1,NULL,WNOHANG)>0){
            /*
            -1: Wait for any child process
            NULL: The second argument stores info about status variable, Null denotes that we do not care about this information
            WNOHANG: Do not block, if the child process has not exited
            */
			printf("Shell : Background Process Finished\n");
		}

        if(!tokens[0]){
			continue; // Nothing provided by user, restart the shell
		}

        // We need to check if the background & is present in the string:

        int background = -1;
        for (int i = 0 ; i < MAX_NUM_TOKENS ; i ++ ) {
            if (tokens[i] == NULL) break;
            else if (!strcmp(tokens[i], "&")) {
                // & detected
                if (tokens[i + 1] == NULL) background = i;
                else {
                    printf("Shell : Multiple Commands after &\n");
					background=-2;
                }
            }
        }
        if (background == -2) {
            // If background is not the last argument, then return
            free_mem(tokens);
			continue;
        }

        if (strcmp(tokens[0], "cd")) { // If the strings are equal then strcmp returns 0, normal comparison might not work, because of different sizes
            // The commands are not cd, so fork a child and use execvp. Since exec changes the memory image, forking a child is necessary
            int pid = fork();
            if (pid == 0) {
                if (background > 0) tokens[background] = NULL; // So that we can pass this array to execvp directly
                execvp(tokens[0],tokens);
				printf("Shell: Incorrect Command : %s\n",tokens[0]);
				exit(0);
            }
            else {
                if (background == -1) waitpid(pid, NULL, 0);
            }
        }
        else {
            // the command is cd, so no need to fork (we want the current main shell to change its default directory)
            if (background > 0) {
                printf("Shell: cd cannot be executed in background :\n");
				free_mem(tokens);
				continue;
            }
            if (!tokens[1] || tokens[2]) {
                // First argument is null, or the second argument contains data
                printf("Shell: Incorrect Command : %s\n",tokens[0]);
            }
            else {
                int ret = chdir(tokens[1]); // to change the directory to the location specified
                if (ret) {
                    printf("Shell: Incorrect Command : %s\n",tokens[0]);
                }
            }
        }
        free_mem(tokens); // This is necessary too
    }
}