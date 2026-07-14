#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int main(void) {
  int fd = open("./test.md", O_CREAT | O_RDWR | O_TRUNC, 0644);

  if (fd == -1) {
    perror("fail to open file");
  }

  char buf[4096] = {0};

  ssize_t nbytes = write(fd, buf, sizeof(buf));
  if (nbytes != sizeof(buf)) {
    perror("fail to write");
  }
}