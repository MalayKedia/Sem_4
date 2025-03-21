#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
// #include <wait.h>
#include <pthread.h>
#include "zemaphore.h"

#define NUM_THREADS 3
#define NUM_ITER 10
zem_t zem_array[NUM_THREADS]; // array of semaphores (one for each thread)

void *justprint(void *data)
{
  int thread_id = *((int *)data); // thread id

  for(int i=0; i < NUM_ITER; i++)
    {
      zem_down(&zem_array[thread_id]); // zem down on the current thread
      printf("This is thread %d\n", thread_id);
      zem_up(&zem_array[(thread_id+1)%NUM_THREADS]); // waking the next thread
    }
  return 0;
}

int main(int argc, char *argv[])
{

  pthread_t mythreads[NUM_THREADS];
  int mythread_id[NUM_THREADS];
  
  for (int i = 0 ; i < NUM_THREADS ; i ++ ) {
    zem_init(&(zem_array[i]), 0); // initializing the semaphores with 0
  }
  zem_up(&zem_array[0]); //waking the first one
  for(int i =0; i < NUM_THREADS; i++)
    {
      mythread_id[i] = i;
      pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
    }
  
  for(int i =0; i < NUM_THREADS; i++)
    {
      pthread_join(mythreads[i], NULL);
    }
  
  return 0;
}