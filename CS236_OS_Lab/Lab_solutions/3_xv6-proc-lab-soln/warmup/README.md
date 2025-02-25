# Solution for warmup exercise of xv6 process management lab

## Files to compulsorily modify to add a syscall
- user.h - Any function that can be called by the user must have its definition added to this file
- usys.S - This file uses a macro to define the assembly code for each syscall. This code is identical for all syscalls and only differs in the number passed into the eax register (ie) `$SYS_***` (found in syscall.h). The basic code to trap into the os for a syscall just loads the approraiate syscall number into the eax register and uses the ```int $T_SYSCALL``` (decides the code that deals with the trap in trap.c) instruction to trap into the OS. 
- sysproc.c - This file contains all the syscalls related to process management. Note that any function that requires to access the ptable is defined in proc.c which has the ptable and is added in defs.h and just called in sysproc. Refer to `fork()`
- syscall.h - Has all the mappings between the syscall macros (make sure the macro is of the form SYS_`name of the syscall`) used in usys.S and the numbers corresponding to them which is pushed into eax when a trap is performed to execute the syscall
- syscall.c - Has the mappings between the values in the eax register during a trap and the syscall that is to be executed
- defs.h - It contains the defintions of all the functions defined in various files of the operating system.

## Files that are commonly modified depending on the syscall
- proc.h - This file contains the definitions of all the process management structures such as struct proc, context structure, cpu. This is to be modified to accomated changes in these structures
- proc.c - This file contains the implementations of existing syscalls that require access to process management structures primarily the ptable
- trap.c - This contains the trap() function which is called during every trap to the OS. A switch case statement is used to execute a different part of the code corresponding to which number was passed to the int n instruction


## Arguments in a syscall
Observing functions in sysproc.c it is clear to see that these do not take arguments directly. The arguments passed to a syscall need to be extracted from the stack directly. To make this more convenient xv6 provides two functions

- `argint(int n, int * ip)` - The $\text{n}^{\text{th}}$ 32 bit (integer) argument  is extracted from the stack and written into the pointer ip. The value can be accessed by dereferencing ip. Returns 0 on success and -1 on failure.
- `argstr(int n, int * ip)` - The $\text{n}^{\text{th}}$ word-sized system call argument is extracted as a pointer to a block of memory of size bytes. It also checks that the pointer lies within the process address space. Returns 0 on success and -1 on failure


These functions can be directly called in any files which includes "defs.h". 


## Steps to add a syscall 

- Add the term `SYSCALL(_name_)` to the end of usys.S, where _name_ is the name of the syscall to be added
- Add the appropriate type signature for the syscall in user.h
- Make a new `#define` for the syscall following the format given in syscall.h, make sure the number given to the syscall is not already used.
- Use extern to add the type signature of sys_ _name_ in syscall.c (extern allows the file to be compiled and the defintion of the function is obtained from all the files linked with it). Also add the mapping from `[SYS_name]` to ` sys_name` in the same file.
- Add the definition of sys_ _name_ in sysproc.c. If any helper functions are defined for this (like in kill()) make sure to define them in the appropriate file (usually proc.c). Make sure to add the definition of any such helper functions in defs.h