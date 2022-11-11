#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char ** argv) {
    if (argc < 2) {
        fprintf(stderr, "not enough args");
        return 1;
    }

    char buff[4096];

    // read from file
    int fd = open(argv[1], O_RDWR);
    int bytes_read = read(fd, buff, sizeof(buff));
    printf("From file '%s': '%.*s'\n", argv[1], bytes_read, buff); 
    printf("Writing 'asdf' to file '%s'\n", argv[1]); 
    int bytes_written = write(fd, "asdf", 4);
    if (-1 == bytes_written ) {
        fprintf(stderr, "error writing to file\n");
        goto fail;
        return 0;
    }

    close(fd); // do not forget to free the resources you're using
    return 0;

fail:
    close(fd); // do not forget to free the resources you're using
    return 1;
}