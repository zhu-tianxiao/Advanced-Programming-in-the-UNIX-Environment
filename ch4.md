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

## access and faccessat Functions
```c
#include <unistd.h>
int access(const char *pathname, int mode);
int faccessat(int fd, const char *pathname, int mode, int flag);
// Both return: 0 if OK, −1 on error
```
## umask Function
The umask function sets the file mode creation mask for the process and returns the previous value. (This is one of the few functions that doesn’t have an error return.)
```c
#include <sys/stat.h>
mode_t umask(mode_t cmask);
// Returns: previous file mode creation mask
```

## chmod, fchmod, and fchmodat Function
The chmod, fchmod, and fchmodat functions allows us to change the file access permissions for an existing file.
```c
#include <sys/stat.h>
int chmod(const char *pathname, mode_t mode);
int fchmod(int fd, mode_t mode);
int fchmodat(int fd, const char *pathname, mode_t mode, int flag);
// All three return: 0 if OK, −1 on error
```

from `<sys/stat.h>`

| mode    | Desciption                              |
| ------- | --------------------------------------- |
| S_ISUID | set-user-ID on execution                |
| S_ISGID | set-group-ID on execution               |
| S_ISVTX | saved-text(sticky bit)                  |
|         |                                         |
| S_IRWXU | read, write, and execute by user(owner) |
| S_IRUSR | read by user                            |
| S_IWUSR | write by user                           |
| S_IXUSR | execute by user                         |
|         |                                         |
| S_IRWXG | read, write, and execute by group       |
| S_IROTH | read by other(world)                    |
| S_IWOTH | write by other(world)                   |
| S_IXOTH | execute by other(world)                 |
## Sticky Bits
`S_ISVTX` bit
## chown, fchown, fchownat, and lchown Functions
The chown functions allow us to change a file’s user ID and group ID, but if either of the arguments owner or group is −1, the corresponding ID is left unchanged.

```c
#include <unistd.h>
int chown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int fchownat(int fd, const char *pathname, uid_t owner, gid_t group,
int flag);
int lchown(const char *pathname, uid_t owner, gid_t group);
// All four return: 0 if OK, −1 on error
```

These four functions operate similarly unless the referenced file is symbolic link. In that case, `lchown` and `fchownat` (with the AT_SYMLINK_NOFOLLOW flag set) change the owner of the symbolic link itself, not the file pointed to by the symbolic link.

The fchown function changes the ownership of the open file referenced by the fd argument. Since it operates on a file that is already open, it can't be used to change the ownership of a symbolic link.

