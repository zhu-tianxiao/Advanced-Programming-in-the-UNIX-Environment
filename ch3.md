# File I/O
## Introduction
Most file I/O on a UNIX system can be performed using only five functions: **open, read, write, lseek, and close**.

unbuffered I/O, in contrast to the standard I/O routines 

The term unbuffered means each read or write invokes a system call in the kernel.

These unbuffered I/O functions are not part of ISO C, but are part of POSIX.1 and the Single UNIX Specification.

Whenever we describe the sharing of resources among multiple processes, the concept of an **atomic** operation becomes important.
## File Descriptors
To the kernel, all open files are refered to by file descriptors. A file desciptor is non-negative integer. When we open an existing file or create a new file, the kernel returns a file descriptor to the process.

By convention, UNIX System shells associate file descriptor with the standard input of a process, file descriptor 0 with the standard input, 1 -> standard output, 2-> standard error. The convention is used by the shells and many applications; it is not a feature of the UNIX kernel.

Although their values are standardized by POSIX.1, the magic numbers 0, 1 and 2 should be replaced in POSIX-compliant applications with the symbolic constants `STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO` to improve readability. These constants are defined in the `unist.d`
## open and openat functions

The path parameter is the name of the file to open or created by calling either the open function or the openat function.
```c
#include <fcntl.h>
int open(const char *path, int oflag, ... /* mode_t mode */ );
int openat(int fd, const char *path, int oflag, ... /* mode_t mode */ );
// Both return: file descriptor if OK, −1 on error
```

For these functions, **the last argument is used only when a new file is being created**, as we describe later.

The path parameter is the name of the file to open or create. This function has a multitude of options, which are specified by the oflag argument. This argument is formed by ORing together one or more of the following constants from the `<fcntl.h>` header:

O_RDONLY
Open for reading only.
O_WRONLY
Open for writing only.
O_RDWR
Open for reading and writing.
O_EXEC
Open for execute only.
O_SEARCH
Open for search only (applies to directories).

这里后两个参数是不在Linux中出现的

One and only one of the previous five constants **must be specified**. The following constants are optional:


The file descriptor returned by open and openat is guaranteed to be the lowest-numbered **unused** descriptor.

The fd parameter distinguishes the openat function from the open function. There are three possibilities:
- The path parameter specifies an absolute pathname. In this case, the fd parameter is ignored and the openat function behaves like the open function.
- The path parameter specifies a relative pathname and the fd parameter is a file descriptor that specifies the starting location in the file system where the relative pathname is to be evaluated. The fd parameter is obtained by opening the directory where the relative pathname is to be evaluated.
- The path parameter specifies a relative pathname and the fd parameter has the special value AT_FDCWD. In this case, the pathname is evaluated starting in the current working directory and the openat function behaves like the open function.

O_APPEND
O_CLOEXEC
O_CREAT: Create the file if it doesn’t exist. This option requires a third argument to the open function (**a fourth argument** to the openat function) — the mode, which specifies the access permission bits of the new file. 
O_DIRECTORY
O_EXCL
O_NOTIFY
O_NOFOLLON
O_NONBLOCK
O_SYNC
O_TRUNC: If the file exists and if it is successfully opened for either write-only or read–write, truncate its length to 0.
O_TTY_INIT


O_DSYNC
O_RSYNC
pass
### Filename and Pathname Truncation
pass

## creat Function
```c
#include <fcntl.h>
int creat(const char *path, mode_t mode);
// Returns: file descriptor opened for write-only if OK, −1 on error
```

the function is equivalent to 
```c
open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
```
One deficiency with creat is that the file is opened only for writing. Before the new version of open was provided, if we were creating a temporary file that we wanted to write and then read back, we had to call creat, close, and then open. A better way is to use the open function, as in


```c
open(path, O_RDWR | O_CREAT | O_TRUNC, mode);
```
## close Function
```c
#include <unistd.h>
int close(int fd);
// Returns: 0 if OK, −1 on error
```
When a process terminates, all of its open files are **closed automatically** by the **kernel**. Many programs take advantage of this fact and don’t explicitly close open files. See the program in Figure 1.4, for example.
## lseek Function
Every open file has an associated ‘‘current file offset,’’ normally a non-negative integer that measures the number of bytes from the beginning of the file. (We describe some exceptions to the ‘‘non-negative’’ qualifier later in this section.) Read and write operations normally start at the current file offset and cause the offset to be incremented by the number of bytes read or written. By default, this offset is initialized to 0 when a file is opened, unless the O_APPEND option is specified.

An open file’s offset can be set explicitly by calling lseek.

```c
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
// Returns: new file offset if OK, −1 on error
```

The interpretation of the offset depends on the value of the whence argument.

- If whence is SEEK_SET, the file’s offset is set to offset bytes from the beginning of the file.
- If whence is SEEK_CUR, the file’s offset is set to its current value plus the offset. The offset can be positive or negative.
- If whence is SEEK_END, the file’s offset is set to the size of the file plus the offset. The offset can be positive or negative.

Because a successful call to lseek returns the new file offset, we can seek zero bytes from the current position to determine the current offset:
```c
off_t currpos;
currpos = lseek(fd, 0, SEEK_CUR);
```

This technique can also be used to determine if a file is capable of seeking. If the file descriptor refers to a pipe, FIFO, or socket, lseek sets errno to ESPIPE and returns −1.

### Example
```c
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
int main(void) {
  if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1) {
    printf("can't seek\n");
  } else {
    printf("seek ok\n");
  }
}
```

```bash
zhu@laptop:/mnt/d/Project/Tutorial/unix/ch3/p67$ ./lseek
can't seek
zhu@laptop:/mnt/d/Project/Tutorial/unix/ch3/p67$ ./lseek < /etc/passwd
seek ok
zhu@laptop:/mnt/d/Project/Tutorial/unix/ch3/p67$ cat /etc/passwd | ./lseek
can't seek
```

lseek only records the current file offset within the kernel—it does not cause any I/O to take place. This offset is then used by the next read or write operation.


### Example
```c
#include "../apue.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void) {
  int fd;
  if ((fd = creat("file.hole", FILE_MODE)) == -1) {
    err_sys("create error");
  }

  if (write(fd, buf1, 10) != 10) {
    err_sys("buf1 write error");
  }

  /* offset now = 10 */

  if (lseek(fd, 16384, SEEK_SET) == -1) {
    err_sys("lseek error");
  }

  if (write(fd, buf2, 10) != 10) {
    err_sys("buf2 write error");
  }
}
```

```bash
zhu@laptop:/mnt/d/Project/Tutorial/unix/ch3/p68$ ./filehole
zhu@laptop:/mnt/d/Project/Tutorial/unix/ch3/p68$ cat file.hole
abcdefghijABCDEFGHIJzhu@laptop:/mnt/d/Project/Tutorial/unix/ch3/p68$ ls -l file.hole
-rwxrwxrwx 1 zhu zhu 16394 May 13 15:54 file.hole
zhu@laptop:/mnt/d/Project/Tutorial/unix/ch3/p68$ od -c file.hole
0000000   a   b   c   d   e   f   g   h   i   j  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
*
0040000   A
```

关于这里空洞文件的数量更少，wsl不支持这个特性

作者提到了大量细节，简单来说，现代系统的`off_t`通常是8个字节，可以支持大文件

## read Function
```c
#include <unisted.h>
ssize_t read(int fd, void *buf, size_t nbytes);
// Returns: number of bytes read, 0 if end of file, −1 on error
```

There are several cases in which the number of bytes actually read is less than the amount requested:
- When reading from a regular file, if the end of file is reached before the requested number of bytes has been read.
- When reading from a terminal device. Normally, up to one line is read at a time.
- When reading from a network. Buffering within the network may cause less than the requested amount to be returned.
- When reading from a network. Buffering within the network may cause less than the requested amount to be returned.
- When reading from a pipe or FIFO. If the pipe contains fewer bytes than requested, read will return only what is available.
- When reading from a record-oriented device. Some record-oriented devices, such as magnetic tape, can return up to a single record at a time.
- When interrupted by a signal and a partial amount of data has already been read. We discuss this further in Section 10.5.


The read operation starts at the file’s current offset. Before a successful return, the offset is incremented by the number of bytes actually read.

## write Function

```c
# include <unistd.h>
ssize_t write(int fd, const void *buf, size_t nbytes);
// Returns: number of bytes written if OK, −1 on error
```

## I/O Efficiency
```c
#include "../apue.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFSIZE 4096
int main(void) {
  int n;
  char buf[BUFFSIZE];

  while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
  {
    if (write(STDOUT_FILENO, buf, n) != n) {
      err_sys("write error");
    } 
  }

  if (n < 0) {
    err_sys("read error");
  }
  
}
```
- It reads from standard input and writes to standard output, assuming that these have been set up by the shell **before this program is executed**. Indeed, all normal UNIX system shells provide a way to open a file for reading on standard input and to create (or rewrite) a file on standard output. **This prevents the program from having to open the input and output files**, and allows the user to take advantage of the shell’s I/O redirection facilities.
- The program doesn’t close the input file or output file. Instead, the program uses the feature of the UNIX kernel that closes all open file descriptors in a process when that process terminates.
- This example works for both text files and binary files, since there is no difference between the two to the UNIX kernel.

## File Sharing

pass

## Atomic Operations
### Appending to a File
古早使用lseek+write，在多进程的条件下会出现问题，因为这两个系统调用何在一起不是原子操作。可以使用`O_APPEND`解决这个问题
### pread and pwrite Functions
```c
#include <unistd.h>
ssize_t pread(int fd, void *buf, size_t nbytes, off_t offset);
// Returns: number of bytes read, 0 if end of file, −1 on error
ssize_t pwrite(int fd, const void *buf, size_t nbytes, off_t offset);
// Returns: number of bytes written if OK, −1 on error
```
Calling pread is equivalent to calling lseek followed by a call to read, with the following exceptions.
- There is no way to interrupt the two operations that occur when we call pread.
- The current file offset is not updated.
### Creating a File
需要通过`O_CREAT | O_EXCL`来保证，检查文件存在和创建合在一起是原子操作。

## dup and dup2 Functions
```c
#include <unistd.h>
int dup(int fd);
int dup2(int fd, int fd2);
// Both return: new file descriptor if OK, −1 on error
```
The new file descriptor returned by dup is guaranted to be the lowest-numbered available file descriptor. 

With dup2, we specify the value of the new descriptor with the fd2 argument. If fd2 is already open, it is first closed. If fd equal fd2, then dup2 returns  fd2 without closing it. Otherwise, the FD_CLOEXEC file descriptor flag is cleared for fd2 so that fd2 is left open if the process calls exec.


The new file descriptor that is returned as the value of the functions shares the same file table entry as the fd argument. We show this in Figure 3.9.

Each descriptor has its own set of file descriptor flags. As we describe in Section 3.14, the close-on-exec file descriptor flag for the new descriptor is always cleared by the dup functions.

如果fd fd2都存在，fd2先关闭，然后复制fd（将fd指向fd2指向的file table entry）；

如果fd存在，fd2不存在，dup2会创建新的fd2，并且复制fd；

如果fd不存在，fd2存在，失败并设置errno（Bad file descriptor），且fd2不会被关闭；

如果fd fd2不存在，失败并设置errno

在前两种正常情况下，fd2会清除原先设置在fd上的FD_CLOEXEC属性。但是，如果`fd==fd2`，dup2会直接返回，并不清除该属性

Another way to duplicate a descriptor is with the `fcntl` function, which we describe in Section 3.14. Indeed, the call `dup(fd);` is equivalent to `fcntl(fd, F_DUPFD, 0);` Similarly, the call `dup2(fd, fd2);` is equivalent to `close(fd2);` `fcntl(fd, F_DUPFD, fd2);`

In this last case, the dup2 is not exactly the same as a close followed by an fcntl. The differences are as follows:
- dup2 is an **atomatic** operation, whereas the alternate form involves two function calls. It is possible in the latter case to have a signal catcher called between the close and the `fcntl` that could modify the file descriptors. The same problem could occur if a different thread changes the file descriptors
- There are some errno differences between dup2 and fctnl.

## sync, fsync, and fdatasync Functions
Traditional implementations of the UNIX System have a buffer cache or page cache in the kernel through which most disk I/O passes. When we write data to a file, the data is normally copied by the kernel into one of its buffers and queued for writing to disk at some later time. This is called **delayed write**.

The kernel eventually writes all the delayed-write blocks to disk, normally when it needs to reuse the buffer for some other disk block. To ensure consistency of the file system on disk with the contents of the buffer cache, the sync, fsync and fdatasync functions are provided.

```c
#include <unistd.h>
int fsync(int fd);
int fdatasync(int fd);
// Returns: 0 if OK, −1 on error
void sync(void);
```

The sync function simply queues all the modified block buffers for writing and returns; it does not wait for the disk writes to take place.

The function sync is normally called periodically (usually every 30 seconds) from a **system daemon**, often called update. This guarantees regular flushing of the kernel’s block buffers. The command sync(1) also calls the sync function.

The function `fsync` refers only to single file, specified by the file descriptor fd, and waits for the disk writes to complete before returning. This function is used when an application, such as a database, needs to be sure that the modified blocks have been written to the disk.

The `fdatasync` function is similar to `fsync`, but it affects only the data portions of a file. With `fsync`, the file's attributes also updated synchronously.
## fcntl Function
