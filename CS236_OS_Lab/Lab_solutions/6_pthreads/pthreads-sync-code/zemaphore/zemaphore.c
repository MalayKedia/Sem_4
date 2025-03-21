#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
// #include <wait.h>
#include "zemaphore.h"

void zem_init(zem_t *s, int value) {
  s->value = value; // initializing the value
  pthread_mutex_init(&(s->lock), NULL);
  pthread_cond_init(&(s->cond), NULL);
}

void zem_down(zem_t *s) {
    pthread_mutex_lock(&(s->lock)); // we are accessing s->value, so lock needed
    if(--(s->value) < 0){ // decrementing the value, and waiting if the value is negative
        pthread_cond_wait(&(s->cond), &(s->lock));
    }
    pthread_mutex_unlock(&(s->lock));
}

void zem_up(zem_t *s) {
    pthread_mutex_lock(&(s->lock));
    s->value++; // incrementing the value
    pthread_cond_signal(&(s->cond)); // everytime, we use up on semaphore, one of the sleeping threads is woken up
    pthread_mutex_unlock(&(s->lock));
}