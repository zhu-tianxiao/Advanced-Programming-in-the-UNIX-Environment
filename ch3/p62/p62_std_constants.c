#include <unistd.h>
#include <stdio.h>
int main(void) {
  printf("standard input: %d\n", STDIN_FILENO),
  printf("standard output: %d\n", STDOUT_FILENO);
  printf("standard error: %d\n", STDERR_FILENO);
}