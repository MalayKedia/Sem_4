#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // for "open" and "O_RDONLY"
#include <unistd.h>     // for "read"
#include <sys/stat.h>   // for "mkfifo"

#define PIPE_NAME "pipe_example"

int main()
{
    char buffer[256];
    mkfifo(PIPE_NAME, 0666); // 0666 - read and write permission for the owner, group, and others
    int fd0 = open(PIPE_NAME, O_RDONLY);

    read(fd0, &buffer, sizeof(buffer));
    printf("Here is the message: %s\n",buffer);
    close(fd0);
    unlink(PIPE_NAME);
}