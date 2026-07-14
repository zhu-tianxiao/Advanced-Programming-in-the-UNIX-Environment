#include "../apue.h"

/**
 * 
 * 
 * 即使什么都不做，系统中的其他程序也会不断创建和销毁进程
 * zhu@unixlab:/mnt/d/Project/Tutorial/unix/ch1/p11_pid$ ./process_id
hello world from process ID 172072
zhu@unixlab:/mnt/d/Project/Tutorial/unix/ch1/p11_pid$ ./process_id
hello world from process ID 172132
 */
int main(void) {
  
  printf("hello world from process ID %ld\n", (long)getpid());
  exit(0);
}