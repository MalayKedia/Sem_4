#include <stdio.h>
#include <pthread.h> // Contains the library functions related to threads

// Macros
#define NUM_THREADS 10 
#define INCREMENTS_PER_THREAD 1000

int counter = 0;  // Global counter, which we will update
pthread_mutex_t lock;  // Mutex lock, can be declared as a global variable

// Function for threads without locking
void* increment_without_lock(void* arg) {
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        counter++;
    }
    return NULL;
}

// Function for threads with locking
void* increment_with_lock(void* arg) {
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        pthread_mutex_lock(&lock); // Acquiring the lock
        counter++;
        pthread_mutex_unlock(&lock); // Releasing the lock
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Run without locking
    counter = 0;
    printf("Running without locking:\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_without_lock, NULL); // Creating the thread
        /*
        Arguments:
            pthread_t *thread: A pointer to a pthread_t variable where the ID of the newly created thread will be stored.
            const pthread_attr_t *attr: Specifies thread attributes. Passing NULL uses the default attributes.
            void *(*start_routine)(void *): A function pointer to the thread's start routine. This function executes when the thread starts.
            void *arg: An argument passed to the start_routine. It allows passing data to the thread. In the code, NULL is passed, indicating no argument.
        */
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); // waits for a specific thread to terminate
        /*
        Arguments:
            pthread_t thread: The ID of the thread to wait for (from pthread_create).
            void **retval: A pointer to retrieve the return value of the thread's start routine. Passing NULL means the return value is ignored.
        */
    }
    printf("Final counter value (without lock): %d\n", counter);

    // Run with locking
    counter = 0;
    pthread_mutex_init(&lock, NULL); // Initializing Mutex
    printf("\nRunning with locking:\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_with_lock, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Final counter value (with lock): %d\n", counter);

    pthread_mutex_destroy(&lock); // Releases any system allocated resources, although typically done by modern OSes, so not necessary
    return 0;
}
