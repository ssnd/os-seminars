#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>


int main() {
    int fd1 = open("hello1.txt", O_RDWR | O_CREAT, 0664);
    if (-1 == fd1) {
        perror("open");
        return 1;
    }

    int fd2 = open("hello2.txt", O_RDWR | O_CREAT, 0664);
    if (-1 ==fd2) {
        perror("open");
        return 1;
    }

    write(fd2, "hello", 5);

    int newfd = dup2(fd1, fd2);
    if (-1  == newfd) {
        perror("dup2");
        return 1;
    }

    // ensure the new fd is equal to the requested one
    assert(newfd == fd2);

    write(fd1, "123", 3);
    write(newfd, "567", 3);

    close(fd1);
    close(newfd);
    return 0;
}