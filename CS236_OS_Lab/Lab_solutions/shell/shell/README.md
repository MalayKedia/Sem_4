# Solutions to the Shell Lab
The problem statement can be found in this directory. The solutions to the exercise are inside the folder "shell-code". The warmup exercises have a separate README, please read that for further explanations.

Here are brief explanations on the various parts:

## Part A: A simple shell
The tokenize function has already been provided. There are two cases here, firstly if the command is ```cd``` or not (for this comparison, we use ```strcmp```, which returns a 0 on equality). If the command is not ```cd```, we will fork and use exec in the child process. In the other case, we use chdir.

## Part B: Background execution
In every iteration of the while loop, we need to check for the presence of any dead children. If there are any, we need to reap them (observe the use of ```WNOHANG``` flag in ```waitpid()``` call). We do not reap the child immediately, in case of background process. Also ```cd``` can't be run in background.

## Part C: The exit command
We need to check for the presence of ```exit``` in the user provided line. If yes, then we use the ```kill()``` system call.

## Part D: Handling the Ctrl+C signal
We will maintain two process group ids, foreground and background. In the ```signal_handler()``` function, we only kill the foreground process group. In case of ```exit```, we need to kill all the groups. The first background process id, is used as the group id of all background processes (see the use of ```setpgid()``` in the code). 

## Part E: Serial and parallel foreground execution
We need to parse the string based on the presence of "&&" and "&&&". In case of "&&", we need to use ```wait(NULL)```, to impose sequentiality. For "&&&", we maintain a separate counter for the number of parallel processes, and then call ```wait(NULL)``` at the end, for each such process.