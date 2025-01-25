#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define FNAME_SIZE 100
#define MAX_FILE_NO 5000
#define BLOCK_SIZE 1024

int main(int argc, char *argv[])
{

  int n, file_no;
  FILE *fp;
  char dest_file_name[FNAME_SIZE];
  char buf[BLOCK_SIZE];

  while(1)
    {
      file_no = rand() % MAX_FILE_NO;

      bzero(dest_file_name, FNAME_SIZE);
      sprintf(dest_file_name, "disk-files/foo%d.pdf", file_no);

      fp = fopen(dest_file_name, "rb");
      if (fp == NULL) {
	perror("Can't open dest file");
	exit(1);
      }
      
      bzero(buf,BLOCK_SIZE);
      while  ( (n = (int)fread( buf, 1, BLOCK_SIZE, fp )) > 0)
	{
	  //do nothing with the read data;
	  bzero(buf,BLOCK_SIZE);
	}
 
       fclose(fp);
    }
}
