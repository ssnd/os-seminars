#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>


int main() {
    int fd = open("hello1.txt", O_RDWR | O_CREAT, 0664);
    if (-1 == fd) {
        perror("open");
        return 1;
    }

    int newfd = dup2(fd, STDOUT_FILENO); 
    if (-1 == newfd) {
        perror("dup2");
        return 1;
    }

    printf("hello world\n");

    close(fd);
    return 0;
}