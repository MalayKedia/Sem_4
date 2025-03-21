#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void)
{
  char* ret;
  printf(1, "Start: memory usage in pages: virtual: %d, physical %d\n", numvp(), numpp());

  ret = sbrk(8192);
  if (!ret) {
    printf(1,"Ret failed\n");
  }
  printf(1, "After sbrk-ing 2 pages: memory usage in pages: virtual: %d, physical %d\n", numvp(), numpp());
 

  ret = sbrk(-4096);
  if (!ret) {
    printf(1,"Ret failed\n");
  }
  printf(1, "After releasing one page: memory usage in pages: virtual: %d, physical %d\n", numvp(), numpp());

  exit();
}
