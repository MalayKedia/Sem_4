#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>


int TOTAL = 10;

/********DO NOT MODIFY********/

#define fifo_w "/tmp/produced"	// for communicating that message that message produced
#define fifo_r "/tmp/consumed"  // for communicating that message that slot is free

int producer_pipe(){
    mkfifo(fifo_w, 0666);
    mkfifo(fifo_r, 0666);
    int fd_w = open(fifo_w, O_WRONLY);
    int fd_r = open(fifo_r, O_RDONLY);
    int pid1 = getpid();
    int pid2 = -1;
    write(fd_w, &pid1, sizeof(pid1));
    read(fd_r,&pid2,sizeof(pid2));
    close(fd_w);
    close(fd_r);
    unlink(fifo_w);
    unlink(fifo_r);
    return pid2;
}

/********DO NOT MODIFY********/

/***TO DO *******/

void sigproduce(int sig){
    
}

void sigdone(int sig){

}


int main()
{
    printf("Producer started\n");
    signal(SIGUSR2,sigproduce);
    signal(SIGUSR1,sigdone);

    const int SIZE = 8;
    const char *name = "OS";
    const char *message0= "present";
    const char *message1   = "emptyyy";

    int pid_consumer = producer_pipe(); //DO NOT EDIT 

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
    
    char free_slot[8];
    int total_produced=0;
    

    // Implement producer logic to send 10 messages
    memcpy(ptr, message1, strlen(message1)+1);
    strncpy(free_slot, ptr, 8);
    printf("Data overwritten in shared memory: %s, Index:%d\n", free_slot, total_produced+1);
    memcpy(ptr, message0, strlen(message0)+1);
    
    


    //Wait for the consumer to signal that it is done
   
    printf("Done\n");

    return 0;
}
