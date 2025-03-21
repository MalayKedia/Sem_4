#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SOCK_PATH "unix_socket_example"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <chunk_size>\n", argv[0]);
        exit(1);
    }

    char *filename = argv[1];
    int chunk_size = atoi(argv[2]);
    if (chunk_size != 128 && chunk_size != 256 && chunk_size != 512 && chunk_size != 1024) {
        fprintf(stderr, "Invalid chunk size. Choose from {128, 256, 512, 1024}\n");
        exit(1);
    }

    int sockfd, filefd;
    struct sockaddr_un serv_addr;
    char buffer[chunk_size];
    struct stat file_stat;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, SOCK_PATH, sizeof(serv_addr.sun_path) - 1);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    printf("Server listening on socket\n");

    struct sockaddr_un cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) error("ERROR on accept");

    filefd = open(filename, O_RDONLY);
    if (filefd < 0) error("ERROR opening file");
    
    if (fstat(filefd, &file_stat) < 0) error("ERROR getting file size");
    off_t file_size = file_stat.st_size;

    // You can use this to read data from a file: read(filefd, buffer, chunk_size);

    // TODO: Read the data from the file and send it to the client in chunks of size chunk_size

    close(filefd);
    close(newsockfd);
    close(sockfd);
    unlink(SOCK_PATH);
    return 0;
}
