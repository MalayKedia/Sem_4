#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define SOCK_PATH "unix_socket_example"
#define MAX_SIZE 256

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_un serv_addr;
    char buffer[MAX_SIZE];

    /* create socket, get sockfd handle */
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* fill in server address */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH); 

    /* getting file name and opening it */
    if(argc != 2)
        error("Usage is <executable> <filename>");

    char *file_name = argv[1];
    int fd = open(file_name, O_RDONLY);
    if(file_name == NULL)
        error("The file doesn't exist");

    bzero(buffer, MAX_SIZE);

    while(read(fd, buffer, MAX_SIZE-1) > 0){

        /* send file data chunk to server */
        n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (n < 0) 
            error("ERROR writing to socket");

        bzero(buffer, MAX_SIZE);
    }

    /* send ending message to server */
    bzero(buffer, MAX_SIZE);    // strlen(buffer) == 0 is the ending message 
    // Another alternative way is to send file size at beginning and determine no of iterations at server 
    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (n < 0) 
        error("ERROR writing to socket");

    close(sockfd);
    return 0;
}