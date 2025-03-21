#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  // This is basic stuff, we will initialize the locks which we are using
  pthread_mutex_init(&(rw->lock), NULL);
  pthread_mutex_init(&(rw->lock_r), NULL);
  rw->num_readers = 0;
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  // num_readers denotes the number of active readers
  pthread_mutex_lock(&(rw->lock_r)); // This lock is for the variable num_readers
  if(rw->num_readers == 0){
    pthread_mutex_lock(&(rw->lock)); // we are the first reader, so we need to get the common lock to avoid writers
  }
  rw->num_readers++; // updating the number of active readers
  pthread_mutex_unlock(&(rw->lock_r)); // the work of this lock is done
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&(rw->lock_r));
  if(--(rw->num_readers) == 0){ // we were the last active reader, so we need to release the common lock for writers
    pthread_mutex_unlock(&(rw->lock));
  }
  pthread_mutex_unlock(&(rw->lock_r));
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&(rw->lock)); // Simple enough, this is the common lock
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_unlock(&(rw->lock)); // Simple enough, this is the common lock
}