#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdbool.h>

using namespace std;

struct read_write_lock
{
    bool is_reader_pref; // boolean so that we can have a common header file for both the files
    int num_readers; // number of readers
    int num_writers; // number of writers
    pthread_mutex_t lock; // A common lock
    pthread_mutex_t lock_r; // Lock for readers
    pthread_mutex_t lock_w; // Lock for writers
    pthread_cond_t cond_r; // Condition variable for readers
    pthread_cond_t cond_w; // Condition variable for writers
};

void InitalizeReadWriteLock(struct read_write_lock * rw);
void ReaderLock(struct read_write_lock * rw);
void ReaderUnlock(struct read_write_lock * rw);
void WriterLock(struct read_write_lock * rw);
void WriterUnlock(struct read_write_lock * rw);