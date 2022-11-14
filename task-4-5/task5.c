#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>

// gcc test.c -o test
// ./task5 test test.txt

int main(int argc, char *argv[])
{
  if (argc < 2) {
    exit(-1);
  }

  char *filename = argv[1];
  char *outname = argv[2];

  printf("%s\n", filename);


  pid_t pid = fork();
  if(pid == 0){
    int file = open(outname, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    dup2(file, 1);
    dup2(file, 2);

    execl(filename, filename, NULL);

    close(file);
  }
  wait(NULL);
}