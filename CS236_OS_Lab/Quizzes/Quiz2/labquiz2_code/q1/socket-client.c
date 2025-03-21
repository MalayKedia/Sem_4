#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define SOCK_PATH "unix_socket_example"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <output_filename> <chunk_size>\n", argv[0]);
        exit(1);
    }

    char *output_filename = argv[1];
    int chunk_size = atoi(argv[2]);
    if (chunk_size != 128 && chunk_size != 256 && chunk_size != 512 && chunk_size != 1024) {
        fprintf(stderr, "Invalid chunk size. Choose from {128, 256, 512, 1024}\n");
        exit(1);
    }

    int sockfd, filefd;
    struct sockaddr_un serv_addr;
    off_t file_size, received_bytes = 0;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, SOCK_PATH, sizeof(serv_addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting to server");

    filefd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (filefd < 0) error("ERROR opening output file");

    // You can use this to write to an output file: write(filefd, buffer, bytes_read);

    // TODO: Read the data from the server and write it to the output file in chunks of size chunk_size

    close(filefd);
    close(sockfd);
    return 0;
}
