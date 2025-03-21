# Solutions to the Pthreads Lab

## Part A: Master-Worker Thread Pool
Please check the master-worker.c file, for the solution to this part. The important things to note are the following:
- No need to produce, if we have reached the buffer limit.
- If we have produced enough items (equal to the total number of items), then we should return anyway.
- Everytime we return, we should make sure that the lock is released.
- When we produce an item, we should wake the consumers.
- When we consume an item, we should wake the producers.

You need to atleast run the script with the following parameters for ensuring correctness of your implementation:
```
./test-master-worker.sh 1000000 1000 500 500
./test-master-worker.sh 10000 1000 500 1
./test-master-worker.sh 10000 1000 1 500
./test-master-worker.sh 1000000 10 500 500
./test-master-worker.sh 1000000 1011 500 500
```

## Part B: Reader-Writer Locks
The question might look tough (especially the preference of writers), but if we break into pieces (various functions), it isn't that tough. Two locks are needed (and sufficient) for both the parts. Solutions are present in the corresponding directory, here are some of the points you need to consider:
- For giving preference to readers, we need to maintain the count of number of readers. If we are the first reader, then we should acquire the big lock (lock common to writers). 
- While updating num_readers, we need to have another lock.
- Giving preference to writers, will involve the use of a condition variable along with two locks. We need to maintain the count of number of writers which are waiting for the lock. 
- If the number of writers are non-zero, we need to wait on the condition variable. This is the step, where we are giving preference to the writers. 
- Again here, we need to acquire the lock, if we are the first reader.
- For updating num_writers, we need another lock.
- When a writer is unlocking and num_writers are 0, we need to broadcast on the condition variable, on which the readers are waiting.

Several other implementations are possible for this question...

## Part C: Semaphores using pthreads
The first part of the question isn't difficult. It is just based on your understanding of semaphores. Please look through the files present in the corresponding directory. Some of the points to consider:
- While accessing the value of the semaphore, acquire the lock.
- Decrement the value during zem_down, and wait if negative.
- While incrementing, signal one of the waiting threads.
- For the second part, we can use an array of zemaphores, one for each of the threads. When we are done with our thread, we wake up the next, and sleep the current one.

## Part D: Your own synchronization problem
You are free to choose any of the problems (refer the book for this). When I had taken the course, I had solved dinner-restaurant problem. (You can also look at the practice problems present on the course website, and implement some of them.)