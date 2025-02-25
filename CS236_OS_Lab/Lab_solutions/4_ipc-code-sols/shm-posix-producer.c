/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.17
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
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
	const char *true_message= "OSisFUN";

	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	/* filling with frees */
	for (int i = 0; i < 512; i++) {
		// sprintf(ptr + 8*i, "%s", free_message);	// This too works
		memcpy(ptr + 8*i, free_message, strlen(free_message)+1);
	}

	mkfifo(fifo_w, 0666);
	mkfifo(fifo_r, 0666);
	int fd_w = open(fifo_w, O_WRONLY);
	int fd_r = open(fifo_r, O_RDONLY);

	int index;
	char free_slot[8];

	int i = 0;
	/* intially, can produce without checking free slots */
	for (; i < SIZE/8; i++) {
		index = 8 * i;
		strncpy(free_slot, ptr + index, 8);
		printf("%d - %s\n", i+1, free_slot);
		memcpy(ptr + index, true_message, strlen(true_message)+1);
		write(fd_w, &index, sizeof(index));
	}

	/* later, read for a free slot and fill in there */
	for (; i < NUM_MSGS; i++) {
		read(fd_r, &index, sizeof(index));
		strncpy(free_slot, ptr + index, 8);
		printf("%d - %s\n", i+1, free_slot);
		memcpy(ptr + index, true_message, strlen(true_message)+1);
		write(fd_w, &index, sizeof(index));
	}

	/* index is -1, when total consumption is done*/
	while(1) {
		read(fd_r, &index, sizeof(index));
		if(index == -1) break;
	}

	close(fd_w);
	close(fd_r);
	unlink(fifo_w);
	unlink(fifo_r);

	return 0;
}