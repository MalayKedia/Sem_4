#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  // Initializing the stuff we need:
  pthread_mutex_init(&(rw->lock), NULL);
  pthread_mutex_init(&(rw->lock_w), NULL);
  pthread_cond_init(&(rw->cond_r), NULL);
  rw->num_writers = 0;
  rw->num_readers = 0;
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&(rw->lock_w)); //Acquiring the lock so that we can use/update num_writers and num_readers
  while(rw->num_writers != 0){
    pthread_cond_wait(&(rw->cond_r), &(rw->lock_w)); // There are some writers waiting, so we need to wait
  }
  if(rw->num_readers == 0){
    pthread_mutex_lock(&(rw->lock)); // We are the first reader, we need to acquire the common lock
  }
  rw->num_readers++; // Updating the number of active readers
  pthread_mutex_unlock(&(rw->lock_w));
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&(rw->lock_w));
  if(--(rw->num_readers) == 0){ // readers are done
    pthread_mutex_unlock(&(rw->lock));
  }
  pthread_mutex_unlock(&(rw->lock_w));
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&(rw->lock_w));
  rw->num_writers++;
  pthread_mutex_unlock(&(rw->lock_w));
  pthread_mutex_lock(&(rw->lock)); // Acquiring the common lock, because we need to write something
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&(rw->lock_w));
  rw->num_writers--;
  if(rw->num_writers == 0){
    pthread_cond_broadcast(&(rw->cond_r)); // writers are done, so we need to wake the sleeping readers
  }
  pthread_mutex_unlock(&(rw->lock_w));
  pthread_mutex_unlock(&(rw->lock)); // releasing the common lock
}