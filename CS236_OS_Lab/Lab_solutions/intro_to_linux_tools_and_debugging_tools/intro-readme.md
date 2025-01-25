1) 

a)Processor here Is the logical CPU. Number of processors is the number of logical CPUs. If hyperthreading is enabled each core can act as multiple logical CPUs. 
cores field represents the number of physical cores in the processor.
b,c,d,e)get info by running lscpu or more /proc/cpuinfo
f) In /proc/meminfo check MemTotal
g) In /proc/meminfo check MemFree
h) In /proc/stat check processes for #of forks and ctxt for context switches

2) 

a,b,c) after running tops command, find process which has max CPU utilization which is and that process is the process coreesponding to running ./cpu. From corresponding row get get PID, CPU%, MEM%, S(State).

3) 

b) ps -o ppid= -p PID gives parent PID of the process. Do this to get all ancestors until it reaches PID 1 which is init process
c)Go to this folder for /proc/PID/fd to get where file descriptors 0,1 and 2 are pointing to.
d) 1 of ./cpu-print process points to grep hello process and 0 of grep process points to cpu-print
e) which command provides executable location

4) check  VSZ   RSS to compare virtual memory and how much is physically resident in memory

5) echo 3 > /proc/sys/vm/drop_caches cleares cache

6) Just run strace command


Debugging Tools

GDB
1) remove line 13

2)  second_last = last;
    last = next;

    swap these 2 lines

Valgrind

Whenever dynamically allocated you need to free it to stop memory leak
Once freed you shouldnt access the memory
you can't free statically allocates memory here 
int arr[10]; where arr is statically allocated and stored in stack

valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./memory_bugs

It should be ./executable-name instead of just executable-name
