#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int num = 5;

int main(void)
{
  printf(1, "Hello, world!\n");
  printf(1,"The number of free pages before I allocate 2 is : %d, call this x\n",getNumFreePages());
  char* mem = sbrk(8192);
  int* tmp = (int*)mem;
  tmp[0] = 5;
  printf(1,"The number of free pages after I allocate 2 is : %d, should be x-2\n",getNumFreePages());
  int n = fork();
  if (n == 0) {
    sleep(50);

    printf(1,"Child 2: Num = %d (should be 5), number of free pages = %d, should be z-2\n",num,getNumFreePages());
    printf(1,"Child 2: Incrementing num\n");
    num++;
    printf(1,"Child 2: Num = %d (should be 6), number of free pages = %d, should be z-2\n",num,getNumFreePages());
    printf(1,"Child 2: mem[0] = %d (should be 5), number of free pages = %d, should be z-2\n",tmp[0],getNumFreePages());
    tmp[0]++;
    printf(1,"Child 2: Incremented mem[0], now it is %d (should be 6), number of free pages = %d, should be z-2\n",tmp[0],getNumFreePages());      
    exit();

  }
  else {
    int r = fork();
    if (r == 0) {

      printf(1,"Child 1: num is %d (should be 5), the number of free pages = %d, call this y < x\n",num,getNumFreePages());
      printf(1,"Child 1: Incrementing num\n");
      num++;
      printf(1,"Child 1: num is %d (should be 6), the number of free pages = %d, should be y-1\n",num,getNumFreePages());
      printf(1,"Child 1: mem[0] = %d (should be 5), number of free pages = %d, should be y-1\n",tmp[0],getNumFreePages());
      tmp[0]++;
      printf(1,"Child 1: Incremented mem[0], now it is %d (should be 6), number of free pages = %d, should be y-2\n",tmp[0],getNumFreePages());
      exit();
    
    }
    else {

      wait();
      printf(1,"Parent: Num = %d (should be 5), number of free pages = %d, call this z < x, z > y\n",num,getNumFreePages());
      printf(1,"Parent: Incrementing num\n");
      num++;
      printf(1,"Parent: Num = %d (should be 6), number of free pages = %d, should be z-1\n",num,getNumFreePages());
      printf(1,"Parent: mem[0] = %d (should be 5), number of free pages = %d, should be z-1\n",tmp[0],getNumFreePages());
      tmp[0]++;
      printf(1,"Parent: Incremented mem[0], now it is %d (should be 6), number of free pages = %d, should be z-2\n",tmp[0],getNumFreePages());     
      
      wait();
      printf(1,"Parent: Num = %d (should be 6), number of free pages = %d, should be x-2\n",num,getNumFreePages());
      printf(1,"Parent: Incrementing num\n");
      num++;
      printf(1,"Parent: Num = %d (should be 7), number of free pages = %d, should be x-2\n",num,getNumFreePages());
      printf(1,"Parent: mem[0] = %d (should be 6), number of free pages = %d, should be x-2\n",tmp[0],getNumFreePages());
      tmp[0]++;
      printf(1,"Parent: Incremented mem[0], now it is %d (should be 7), number of free pages = %d, should be x-2\n",tmp[0],getNumFreePages());     
      exit();

    }
  }
}
