// 这种方法是糟糕的
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define BUFFSIZE 10
int main(void) {
  int fd = open("./copy_file.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
  if (fd == -1) {
    perror("open file failed!");
    exit(1);
  }

  char buf[BUFFSIZE + 1] = "aaaaaaaaaa";
  int n = 0;
  while (n++ < 51658176) {
    if (write(fd, buf, BUFFSIZE) != BUFFSIZE) {
      perror("write error!");
      exit(1);
    }
  }
}