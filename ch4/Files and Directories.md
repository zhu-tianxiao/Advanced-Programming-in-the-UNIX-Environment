## Introduction
stat function 

stat member

attributes of a file

modify these attributes: change the owner, change the permissions...

structure of a UNIX file system

symbolic links

functions that operate on directories

function that descends through a directory hierachy

## stat, fstat, fstatat, and lstat Functions
```c
#include <sys/stat.h>
int stat(const char *restrict pathname, struct stat *restrict buf );
int fstat(int fd, struct stat *buf );
int lstat(const char *restrict pathname, struct stat *restrict buf );
int fstatat(int fd, const char *restrict pathname,
struct stat *restrict buf, int flag);
// All four return: 0 if OK, −1 on error
```
## File Types
- Regular file.
- Directory file.
- Block special file.
- Character special file.
- FIFO.
- Socket.
- Symbolic link.

## Set-User-ID and Set-Group-ID
Every process has six or more IDs associated with it.


|                                       |                                        |
| ------------------------------------- | -------------------------------------- |
| real user ID, real group id           | who you really are                     |
| effective user ID, effective group ID | used for file access permission checks |
| saved set-user-ID, saved set-group-ID | saved by exec function                 |

Normally, the effective user ID equals the real user ID, and the effective group ID equals the real group ID.

Returning to the stat function, the set-user-ID bit and the set-group-ID bit are contained in the file's st_mode value. These two bits can be tested against the constants `S_ISUID` and `S_ISGID`, respectively.

## File Access Permissions
The file access tests that the kernel performs each time  a process opens, creates, or deletes a file depend on the owners of the file(st_uid and st_gid), the effective IDs of the process (effective user ID and effective group ID), and the supplementary group IDs of the process, if supported. The two owner IDs are properties of the file, whereas the two effective IDs and the supplementary group IDs are properties of the process.

## Ownership of New Files and Directories
The user ID of a new file is set to the effective user ID of the process.

POSIX.1 allows an implementation to choose one of the following options to determine the group ID of a new file:
- The group ID of a new file can be the effective group ID of the process.
- The group ID of a new file can be the group ID of the directory in which the file is being created.

## 