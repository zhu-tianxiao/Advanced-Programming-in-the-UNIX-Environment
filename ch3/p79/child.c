#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int ret = fcntl(3, F_GETFD);

    if (ret == -1) {
        printf("fd 3 does NOT exist\n");
    } else {
        printf("fd 3 still exists after exec\n");

        int flags = fcntl(3, F_GETFD);

        if (flags & FD_CLOEXEC)
            printf("FD_CLOEXEC is set\n");
        else
            printf("FD_CLOEXEC is cleared\n");
    }

    return 0;
}