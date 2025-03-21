#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_cond_t cond; // Declares a condition variable for thread synchronization
pthread_mutex_t lock; // Declares a mutex lock

int current = 0; // Shared variable to track the current thread's turn to execute

void* inc(void* arg){
    sleep(rand() % 10 + 1); // Introduces a random delay (1 to 10 seconds) for the thread
    int i = *(int*)arg; // Retrieves the thread's unique ID
    pthread_mutex_lock(&lock); // Acquire lock
    while(current != i){ // Wait until it is our turn
        pthread_cond_wait(&cond, &lock); // block the thread till then
    }
    printf("I am thread %d\n", i);
    current++; // Increment the shared counter to help the successor
    pthread_cond_broadcast(&cond); // Waking up all threads
    pthread_mutex_unlock(&lock); // Release the lock
    return NULL;
}

int main(){
    printf("Number of threads: ");
    pthread_cond_init(&cond, NULL); // Initializes the condition variable
    pthread_mutex_init(&lock, NULL); // Initializes the mutex lock
    int N; // Number of threads as input
    scanf("%d", &N);
    pthread_t ids[N];
    int args[N];
    for(int i = 0; i < N; i++){
        args[i] = i;
        pthread_create(&(ids[i]), NULL, inc, &(args[i]));
    }
    for(int i = 0; i  < N; i++){
        pthread_join(ids[i], NULL);
    }
    printf("I am the main thread\n");
}