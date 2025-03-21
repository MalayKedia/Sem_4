
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

int consumer_pipe(){
    int fd_w = open(fifo_w, O_RDONLY);
    int fd_r = open(fifo_r, O_WRONLY);

    if(fd_w < 0 || fd_r < 0) {
        printf("Producer not running, Please Try again!\n");
        exit(-1);
    }
    
    int pid1 = getpid();
    int pid2 = -1;
    read(fd_w, &pid2, sizeof(pid2));
    write(fd_r,&pid1,sizeof(pid1));

    return pid2;
}
/********DO NOT MODIFY********/


/***TO DO *******/


void sigconsume(int sig){
}

int main()
{
    printf("Consumer started\n");
    signal(SIGUSR1,sigconsume);

    
    const int SIZE = 8;
    const char *name = "OS";
    const char *message0= "present";
    const char *message1   = "emptyyy";
 
    
    int pid_producer = consumer_pipe(); //DO NOT EDIT

    int shm_fd;
    void *ptr;

    printf("Pid's of producer, consumer communicated!\n");

    int consume_time = 0;
    printf("Enter the time taken to consume a string (in seconds): ");
    scanf("%d", &consume_time);
 
    /* open the shared memory segment */
    shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("shared memory failed\n");
        exit(-1);
    }
 
    /* now map the shared memory segment in the address space of the process */
    ptr = mmap(0,SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        exit(-1);
    }
 
    int total_consumed = 0;
    char consumed_message[8];
    

    //Implement consumer logic to consume 10 messages
    strncpy(consumed_message, ptr, 8);
    printf("Data consumed from shared memory: %s, Index: %d\n",consumed_message,total_consumed+1);
    memcpy(ptr, message1, strlen(message1)+1);
    sleep(consume_time);



    //Communicate that you are done consumption to the producer

    /* remove the shared memory segment */
    if (shm_unlink(name) == -1) {
        printf("Error removing %s\n",name);
        exit(-1);
    }
    
    printf("Done\n");
    return 0;
 }
 
 