#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_cond_t cond;
pthread_mutex_t lock;

int current = 0;

void* inc(void* arg){
    int i = ((int*)arg)[0];
    int N = ((int*)arg)[1]; // This N can also be used as a global variable, but just to show how to pass arrays
    while(1){ // We need to do this indefinitely
        pthread_mutex_lock(&lock);
        while(current != i){
            pthread_cond_wait(&cond, &lock);
        }
        printf("%d, ", i + 1);
        current = (current + 1) % N; // Notice the use of %N (wasn't present in the previous warmup)
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(){
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&lock, NULL);
    int N;
    scanf("%d", &N);
    pthread_t ids[N];
    int args[N][2];
    for(int i = 0; i < N; i++){
        args[i][0] = i;
        args[i][1] = N;
        pthread_create(&(ids[i]), NULL, inc, &(args[i]));
    }
    for(int i = 0; i  < N; i++){
        pthread_join(ids[i], NULL);
    }
    printf("I am the main thread\n");
}