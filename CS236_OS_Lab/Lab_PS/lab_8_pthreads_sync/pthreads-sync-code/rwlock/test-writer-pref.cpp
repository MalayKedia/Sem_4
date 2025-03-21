#include "rwlock.h"
#include <ctime>
#include <algorithm>

using namespace std;

long index;
long *readerAcquireTime;
long *readerReleaseTime;
long *writerAcquireTime;
long *writerReleaseTime;
bool testCasePassed;
struct read_write_lock rwlock;
pthread_spinlock_t spinlock;

void *Reader(void* arg)
{
	int threadNUmber = *((int *)arg);

	// Occupying the Lock
	ReaderLock(&rwlock);

	pthread_spin_lock(&spinlock);
	readerAcquireTime[threadNUmber] = index;
	index++;
	pthread_spin_unlock(&spinlock);

	// printf("Reader: %d has acquired the lock\n", threadNUmber);
	usleep(100000);

	pthread_spin_lock(&spinlock);
	readerReleaseTime[threadNUmber] = index;
	index++;
	pthread_spin_unlock(&spinlock);

	// Releasing the Lock
	ReaderUnlock(&rwlock);
	// printf("Reader: %d has released the lock\n",threadNUmber);
}

void *Writer(void* arg)
{
	int threadNUmber = *((int *)arg);

  // Occupying the Lock
	WriterLock(&rwlock);

	pthread_spin_lock(&spinlock);
	writerAcquireTime[threadNUmber] = index;
	index++;
	pthread_spin_unlock(&spinlock);

	// printf("Writer: %d has acquired the lock\n",threadNUmber);
  usleep(100000);

	pthread_spin_lock(&spinlock);
	writerReleaseTime[threadNUmber] = index;
	index++;
	pthread_spin_unlock(&spinlock);

	// Releasing the Lock
	WriterUnlock(&rwlock);
	// printf("Writer: %d has released the lock\n",threadNUmber);
}

int main(int argc, char *argv[])
{

	int *threadNUmber;
	pthread_t *threads;

	InitalizeReadWriteLock(&rwlock);

	int read_num_threads;
	int write_num_threads;

	read_num_threads = atoi(argv[1]);
	write_num_threads = atoi(argv[2]);

	index = 0;
	readerAcquireTime = new long[read_num_threads*2];
	readerReleaseTime = new long[read_num_threads*2];
	writerAcquireTime = new long[write_num_threads];
	writerReleaseTime = new long[write_num_threads];
	pthread_spin_init(&spinlock, 0);

	int num_threads = 2*read_num_threads + write_num_threads;

	threads = (pthread_t*) malloc(num_threads * (sizeof(pthread_t)));

	int count = 0;
	for(int i=0;i<read_num_threads;i++)
	{

		int *arg = (int *)malloc((sizeof(int)));
		if (arg == NULL)
		{
			printf("Couldn't allocate memory for thread arg.\n");
			exit(EXIT_FAILURE);
		}
		*arg = i;
		int ret = pthread_create(threads+count,NULL,Reader,(void*) arg);
		if(ret)
    {
        printf("Error - pthread_create() return code: %d\n",ret);
        exit(EXIT_FAILURE);
    }
		count++;
	}

	for(int i=0;i<write_num_threads;i++)
	{
		int *arg = (int *)malloc((sizeof(int)));
		if (arg == NULL)
		{
			printf("Couldn't allocate memory for thread arg.\n");
			exit(EXIT_FAILURE);
		}
		*arg = i;
		int ret = pthread_create(threads+count,NULL,Writer,(void*) arg);
		if(ret)
    {
        printf("Error - pthread_create() return code: %d\n",ret);
        exit(EXIT_FAILURE);
    }
		count++;
	}
	usleep(500);

	for(int i=0;i<read_num_threads;i++)
	{

		int *arg = (int *)malloc((sizeof(int)));
		if (arg == NULL)
		{
			printf("Couldn't allocate memory for thread arg.\n");
			exit(EXIT_FAILURE);
		}
		*arg = read_num_threads + i;
		int ret = pthread_create(threads+count,NULL,Reader,(void*) arg);
		if(ret)
    {
        printf("Error - pthread_create() return code: %d\n",ret);
        exit(EXIT_FAILURE);
    }
		count++;
	}

	for(int i=0;i<num_threads; i++)
		pthread_join(threads[i],NULL);

	// TESTING SCRIPT 
	// for(int i=0; i<read_num_threads*2; i++)
	// 	printf("Reader %d Lock Time: %ld Unlock Time: %ld\n", i, readerAcquireTime[i], readerReleaseTime[i]);

	// for (int i = 0; i < write_num_threads; i++)
	// 	printf("Writer %d Lock Time: %ld Unlock Time: %ld\n", i, writerAcquireTime[i], writerReleaseTime[i]);

	long *max_reader_acquire_time_first_half = max_element(readerAcquireTime, readerAcquireTime + read_num_threads);
	long *min_reader_release_time_first_half = min_element(readerReleaseTime, readerReleaseTime + read_num_threads);
	long *max_reader_release_time_first_half = max_element(readerReleaseTime, readerReleaseTime + read_num_threads);
	long *min_reader_acquire_time_second_half = min_element(readerAcquireTime + read_num_threads, readerAcquireTime + 2 * read_num_threads);
	long *max_reader_acquire_time_second_half = max_element(readerAcquireTime + read_num_threads, readerAcquireTime + 2 * read_num_threads);
	long *min_reader_release_time_second_half = min_element(readerReleaseTime + read_num_threads, readerReleaseTime + 2 * read_num_threads);
	long *min_writer_acquire_time = min_element(writerAcquireTime, writerAcquireTime + write_num_threads);
	long *max_writer_release_time = max_element(writerReleaseTime, writerReleaseTime + write_num_threads);

	// not necessary
	// // check if all readers get lock immediately in first half
	// if  ((read_num_threads > 0) && (*max_reader_acquire_time_first_half > *min_reader_release_time_first_half)){
	// 	printf("Reader should not wait to acquire lock in first half\n");
	// 	exit(0);
	// }

	// check if all readers get lock immediately in second half
	if  ((read_num_threads > 0) && (*max_reader_acquire_time_second_half > *min_reader_release_time_second_half)){
		printf("Reader should not wait to acquire lock in second half\n");
		exit(0);
	}

	// not necessary
	// check if no reader is holding lock after writer has held lock
	// if  ((read_num_threads > 0) && (write_num_threads > 0) && (*max_reader_release_time_first_half > *min_writer_acquire_time)){
	// 	printf("Reader can not hold lock when writer has acquired a lock\n");
	// 	exit(0);
	// }

	// check if no reader is holding while a writer is still holding lock
	if ((read_num_threads > 0) && (write_num_threads > 0) && (*min_reader_acquire_time_second_half < *max_writer_release_time)){
		printf("Reader can not acquire lock when writer is holding a lock\n");
		exit(0);
	}

	// check if writer exited immediately
	for (int i = 0; i < write_num_threads; i++)
		if ((writerReleaseTime[i] - writerAcquireTime[i]) != 1)
		{
			printf("No reader/ writer is allowed when a writer holds lock\n");
			exit(0);
		}

	// cout << *max_reader_release_time_first_half << endl;
	// cout << *min_reader_acquire_time_second_half << endl;
	// cout << *min_writer_acquire_time << endl;
	// cout << *max_writer_release_time << endl;
	printf("PASSED\n");
}
