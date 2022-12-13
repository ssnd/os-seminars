#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main() {
    printf("before exec, pid=%d\n", getpid());

    close(STDOUT_FILENO); 
    int fd_out = open("new_stdout.txt", O_RDWR|O_CREAT, 0640);
    printf("new stdout=%d\n", fd_out); // the fds should be the same though

    execlp("python", "python", "-c", "import os; print(f\"hello, input={input()}, pid={os.getpid()}\")", NULL);

    // not coming back unless something bad happens
    perror("something bad happened");
    return 1;
}