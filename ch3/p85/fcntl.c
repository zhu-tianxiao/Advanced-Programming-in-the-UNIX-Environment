#include "../apue.h"
#include <fcntl.h>

void set_fl(int fd, int flags) /* flags are file status flags to turn on */
{
  int val;
  if ((val = fcntl(fd, F_GETFL, 0)) < 0)
    err_sys("fcntl F_GETFL error");
  val |= flags;
  /* turn on flags */
  if (fcntl(fd, F_SETFL, val) < 0)
    err_sys("fcntl F_SETFL error");
}

int main(void) {
  int file = open("test.foo", O_RDWR | O_CREAT, 0644);

  if (file == -1) {
    err_quit("open error");
  }

  set_fl(file, O_APPEND);

  int val;
  if ((val = fcntl(file, F_GETFL, 0)) < 0) {
    err_sys("fcntl error for fd %d", file);
  }
  switch (val & O_ACCMODE) {
  case O_RDONLY:
    printf("read only");
    break;
  case O_WRONLY:
    printf("write only");
    break;

  case O_RDWR:
    printf("read write");
    break;
  default:
    err_dump("unknown access mode");
    break;
  }

  if (val & O_APPEND) {
    printf(", append");
  }

  if (val & O_NONBLOCK) {
    printf(", nonblocking");
  }

  if (val & O_SYNC) {
    printf(", synchronous writes");
  }

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
  if (val & O_FSYNC) {
    printf(", synchronous writes");
  }
#endif
  putchar('\n');
}