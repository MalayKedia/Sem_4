#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

    while (1) {
        /* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line)); // Set all elements as 0
		printf("$ "); // The initial character, which asks for user prompt
		scanf("%[^\n]", line); // Everything except "\n" is accepted
		getchar(); // For taking the "\n" from user
        line[strlen(line)] = '\n'; //terminate with new line
        tokens = tokenize(line);

        if(!tokens[0]){
			continue; // Nothing provided by user, restart the shell
		}

        if (strcmp(tokens[0], "cd")) { // If the strings are equal then strcmp returns 0, normal comparison might not work, because of different sizes
            // The commands are not cd, so fork a child and use execvp. Since exec changes the memory image, forking a child is necessary
            if (fork() == 0) {
                execvp(tokens[0],tokens);
				printf("Shell: Incorrect Command : %s\n",tokens[0]);
				exit(0);
            }
            else {
                wait(NULL); // parent reaps child
            }
        }
        else {
            // the command is cd, so no need to fork (we want the current main shell to change its default directory)
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