#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    int fd = open("hello.txt1", O_RDWR | O_CREAT, 0664);
    if (-1 == fd) {
        perror("open");
        return 1;
    }

    int newfd = dup(fd);
    if (-1  == newfd) {
        perror("dup");
        return 1;
    }

    write(fd, "123", 3);
    write(newfd, "345", 3);

    close(fd);
    close(newfd);
    return 0;
}