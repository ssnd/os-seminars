#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>


// source: https://github.com/yuri-pechatnov/caos/tree/master/caos_2020-2021/sem10-low-level-io
int read_retry(int fd, char* data, int size) {
    char* cdata = data;
    while (1) {
        int read_bytes = read(fd, cdata, size);
        if (read_bytes == 0) { 
            return cdata - data;
        }

        if (read_bytes < 0) { 
            if (errno == EAGAIN || errno == EINTR) { 
                continue;
            } else { 
                return -1;
            }
        }
        cdata += read_bytes;
        size -= read_bytes;
        if (size == 0) {
            return cdata - data;
        }
    }
}
// (echo -n "A"; sleep 1; echo -n "B" ) | ./a.out

int main(int argc, char ** argv) {
    // read from stdin (scanf?)
    char buff[4096];
    int bytes_read = read(STDIN_FILENO, buff, sizeof(buff));
    printf("From stdin: '%.*s'\n", bytes_read, buff); // why not %s?

    return 0;
}