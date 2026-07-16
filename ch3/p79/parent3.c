#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int fd = open("test.txt", O_CREAT | O_RDWR, 0644);
  if (fd == 3) {
    printf("fd == 3\n");
  }
  // 
  

  // 设置 close-on-exec
  fcntl(fd, F_SETFD, FD_CLOEXEC);
  dup2(fd, 3);

  printf("exec now...\n");

  execl("./child", "child", NULL);

  perror("execl");

  return 0;
}