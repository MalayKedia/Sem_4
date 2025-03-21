# Solutions to Pthreads Lab Warmup exercises
## 1
The solution to this question is present in the file 1.c.

```terminal
gcc 1.c -lpthread ; ./a.out                                             ─╯                        
Running without locking:
Final counter value (without lock): 9000

Running with locking:
Final counter value (with lock): 10000
```
Use this exercise, to learn how to lock, initialize, destroy and unlock mutexes and creating and joining threads. The counter value without locks is variable across various operating systems (you might observe a 10000 everytime).

## 2
The solution to this question is present in the file 2.c.
The important part of this question was how to pass integer variables to thread initializing functions. Also the essence of passing different variables to each function call (rather than passing the iteration variable directly).

## 3
The solution to this question is present in the file 3.c. Use this question to learn how to initialize and use condition variables.

## 4
The solution to this question is present in the file 4.c. Quite similar to the previous exercise, just use an infinite while loop.