#include <stdio.h>
#include <pthread.h>


void* inc(void* arg){ // arg is a generic pointer
    int i = *(int*)arg; // casting to int* and then dereferencing
    printf("I am thread %d\n", i);
    return NULL;
}

int main(){
    printf("Number of threads: ");
    int N;
    scanf("%d", &N); // Number of threads to create
    pthread_t ids[N]; // thread identifiers
    int args[N]; // arguments to threads
    for(int i = 0; i  < N; i++){
        args[i] = i;    // each thread assigned a unique identifier (because i is changing)
        pthread_create(&(ids[i]), NULL, inc, &(args[i]));
    }
    for(int i = 0; i  < N; i++){
        pthread_join(ids[i], NULL);
    }
    printf("I am the main thread\n");
}