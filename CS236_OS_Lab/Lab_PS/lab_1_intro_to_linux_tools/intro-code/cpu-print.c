#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

int main(int argc, char *argv[]) 

{
  unsigned int i;
  int count = 0;
  struct timeval tv;
  
  while(1)
    {
      for(i = 0; i < 1000000; i++)
	{
	  gettimeofday(&tv, NULL);
	  printf("%lu sec, %lu usec\n", tv.tv_sec, tv.tv_usec);
	}
      count++;
      printf("round %d complete\n", count);
    }
}
