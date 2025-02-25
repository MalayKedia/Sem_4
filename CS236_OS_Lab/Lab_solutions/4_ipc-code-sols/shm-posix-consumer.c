/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>

#define MAX_SIZE 20
#define fifo_w "/tmp/produced"	// for communicating that message that message produced
#define fifo_r "/tmp/consumed"  // for communicating that message that slot is free

int main()
{
	const int SIZE = 4096;
	const int NUM_MSGS = 1000;
	const char *name = "OS part A";
	const char *free_message= "freeeee";

	int shm_fd;
	void *ptr;
	int i;

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDWR, 0666);	// O_RDONLY -> ORDWR
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);	// PROT_READ -> PROT_READ | PROT_WRITE
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	int fd_w = open(fifo_w, O_RDONLY);
	int fd_r = open(fifo_r, O_WRONLY);
	if(fd_w < 0 || fd_r < 0) {
		printf("Pipes not created yet");
		exit(-1);
	}

	int index;
	char consumed_message[8];

	/* read for index of produced message, then free the slot and send the index*/
	for (int i = 0; i < NUM_MSGS; i++) {
		read(fd_w, &index, sizeof(index));

		strncpy(consumed_message, ptr + index, 8);
		printf("%d - %s\n", i+1, consumed_message);
		memcpy(ptr + index, free_message, strlen(free_message)+1);

		write(fd_r, &index, sizeof(index));

		// sleep(1);
	}

	index = -1;
	write(fd_r, &index, sizeof(index));

	/* remove the shared memory segment */
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}

	return 0;
}
