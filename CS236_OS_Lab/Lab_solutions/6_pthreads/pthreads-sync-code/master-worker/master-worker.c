#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
// #include <wait.h>
#include <pthread.h>
#include <assert.h>

int item_to_produce, curr_buf_size;  // item_to_produce = number of items produced so far, cur_buf_size = current buffer filled
int total_items, max_buf_size, num_workers, num_masters; // total_items = total number of items to be produced, max_buf_size = max buffer size

int *buffer; // buffer array pointer

pthread_cond_t cond_p; // condition variable for producers
pthread_cond_t cond_c; // condition variable for consumers
pthread_mutex_t lock; // common lock for buffer

void print_produced(int num, int master)
{
    printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker)
{

    printf("Consumed %d by worker %d\n", num, worker);
}

void *generate_requests_loop(void *data)
{
    // For producing requests to buffer
    int thread_id = *((int *)data); // unique thread id

    while(1){
        pthread_mutex_lock(&lock); // Acquiring the buffer lock
        while(curr_buf_size >= max_buf_size){
            pthread_cond_wait(&cond_p, &lock); // buffer size full
        }
        if(item_to_produce >= total_items){
            pthread_cond_broadcast(&cond_p); // items already produced, so wake up all producers (so that they can return too)
            pthread_mutex_unlock(&lock); // release the lock, before returning
            return 0;
        }
        else{
            buffer[curr_buf_size++] = item_to_produce; // add to the buffer
            print_produced(item_to_produce, thread_id);
            item_to_produce++;
            pthread_cond_broadcast(&cond_c); // Wake up all consumers, because we have got one element
            pthread_mutex_unlock(&lock);// Release the lock
        }
    }
    return 0;
}

void *consume_requests_loop(void *data)
{
    int thread_id = *((int *)data); // Unique thread id

    while(1){
        pthread_mutex_lock(&lock); // Acquire buffer lock
        while(curr_buf_size == 0){ // Buffer size 0, which means no element to consume
            if(item_to_produce >= total_items){
                pthread_cond_broadcast(&cond_c); // All items produced, so we should wake consumers
                pthread_mutex_unlock(&lock); // Release the lock
                return 0;
            }
            else{
                pthread_cond_wait(&cond_c, &lock); // Wait for production
            }
        }
        int ent = buffer[--curr_buf_size]; // Get the element
        print_consumed(ent, thread_id);

        pthread_cond_broadcast(&cond_p); // One element extracted out, so wake up producers
        pthread_mutex_unlock(&lock); // Release the lock again
    }

    return 0;
}

// write function to be run by worker threads
// ensure that the workers call the function print_consumed when they consume an item

int main(int argc, char *argv[])
{
    pthread_cond_init(&cond_c, NULL);
    pthread_cond_init(&cond_p, NULL);
    pthread_mutex_init(&lock, NULL);

    int *master_thread_id;
    int* worker_thread_id;
    pthread_t *master_thread;
    pthread_t *worker_thread;
    item_to_produce = 0;
    curr_buf_size = 0;

    int i;

    if (argc < 5)
    {
        printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
        exit(1);
    }
    else
    {
        num_masters = atoi(argv[4]);
        num_workers = atoi(argv[3]);
        total_items = atoi(argv[1]);
        max_buf_size = atoi(argv[2]);
    }

    buffer = (int *)malloc(sizeof(int) * max_buf_size);

    // create master producer threads
    master_thread_id = (int *)malloc(sizeof(int) * num_masters);
    master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
    for (i = 0; i < num_masters; i++){
        master_thread_id[i] = i;
    }

    for (i = 0; i < num_masters; i++){
        pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
    }

    // create worker consumer threads
    worker_thread_id = (int *)malloc(sizeof(int) * num_workers);
    worker_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_workers);
    for (i = 0; i < num_workers; i++){
        worker_thread_id[i] = i;
    }
    for (i = 0; i < num_workers; i++){
        pthread_create(&worker_thread[i], NULL, consume_requests_loop, (void *)&worker_thread_id[i]);
    }

    // wait for all threads to complete
    for (i = 0; i < num_masters; i++)
    {
        pthread_join(master_thread[i], NULL);
        printf("master %d joined\n", i);
    }

    for (i = 0; i < num_workers; i++)
    {
        pthread_join(worker_thread[i], NULL);
        printf("worker %d joined\n", i);
    }

    /*----Deallocating Buffers---------------------*/
    free(buffer);
    free(master_thread_id);
    free(master_thread);

    return 0;
}