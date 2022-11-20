#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main () {
    printf("opening...\n");
    int fd = open("some_file.txt", O_RDWR);
    char ch;
    int res;
    while (1) {
        if (!(res = read(fd, &ch, 1))) {
            lseek(fd, 0, SEEK_SET);
            printf("starting over...\n");
            continue;
        }

        printf("next char=%c\n", ch);
        sleep(1);
    }
}