#include <pthread.h>

typedef struct zemaphore {
    int value; // The current value of the semaphore
    pthread_mutex_t lock; // the lock
    pthread_cond_t cond; // the condition variable
} zem_t;

// the function definitions
void zem_init(zem_t *, int);
void zem_up(zem_t *);
void zem_down(zem_t *);