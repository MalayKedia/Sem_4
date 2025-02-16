#include "types.h"
#include "stat.h"
#include "user.h"

void delay(int count)
{
    int i;
    int j, k;
    int *data;

    data = (int *)malloc(sizeof(int) * 1024 * 10);
    if (data <= 0)
        printf(1, "Error on memory allocation \n");

    for (i = 0; i < count; i++)
    {
        for (k = 0; k < 5700; k++)
            for (j = 0; j < 1024 * 10; j++)
                data[j]++;
    }
}
int main(int argc, char *argv[])
{

  int N = 5; //5 children, please change and test with other values also

  int pids[N];
  int rets[N];
  setprio(1000);
  printf(1, "Priority of parent process = %d\n", getprio());

  for (int i = 0; i < N; i++)
    {
      // Set process priority
      // Change priority of children in different order
      // and verify your implementations
      // this example sets higher priority to later children
      // so you should expect children to exit in reverse order of their creation
      
      int priority = 10*i + 1 ;
      
      int ret = fork();
      if (ret == 0)
        {
	  setprio(priority);
	  delay(20);
	  exit();
        }
      else if (ret > 0)
        {
	  pids[i] = ret;
        }
      else
        {
	  printf(1, "fork error \n");
	  exit();
        }
    }
  
  for (int i = 0; i < N; i++)
    {
      rets[i] = wait();
    }
  
  printf(1, "\nAll children completed\n");
  for (int i = 0; i < N; i++)
    printf(1, "Child %d.    pid %d\n", i, pids[i]);
  
  printf(1, "\nExit order \n");
  for (int i = 0; i < N; i++)
    printf(1, "pid %d\n", rets[i]);
  
  exit();
}
