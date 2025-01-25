# Solutions to Shell Lab Warmup exercises
## 1
The solution to this question is present in the file 1.c.

The order in which the print statements are printed depends on the system, you are using. Like, when I run on MacOS, the parent is printed first always. But on other systems, the order varies.

To guarantee that the parent always prints after the child, the parent should wait for the child process to terminate using the ```wait``` system call.


## 2
The solution to this question is present in the file 2.c.

## 3
The solution to this question is present in the file 3.c. Look through other variants of ```exec``` system call, and see if something else too suits here.

## 4
The solution to this question is present in the file 4.c.

## 5
The print statement before the exec system call will always be printed. If the exec system call is successful, then the print statement after it won't get printed (because the process is replaced by a new image). However if the exec call is provided with invalid arguments, then this print statement will also be executed.

## 6
The most important part of the question was using the "correct" number of wait calls. Please check the role of the variable ```x``` in 6.c.

As you would have predicted, the total number of processes created are ```pow(2, n)```. The following diagram shows the hierarchy for n = 3:

```
            P
            |--C1
               |--C11
                   |--C111
               |--C12
            |--C2
                |--C21
            |--C3
```
Basically, the value of ```i``` (for loop iteration variable) remains the same when a child is forked, and the child continues thereafter. Dry run the code for ```n=2,3```, and things will become clear thereafter.

## 7
The solution to this question is present in the file 7.c.

## 8
The solution to this question is present in the file 8.c. 