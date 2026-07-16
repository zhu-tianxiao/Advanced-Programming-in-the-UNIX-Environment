#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
  dup2(4,5);
  perror("dup2");
}