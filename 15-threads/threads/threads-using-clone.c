#define _GNU_SOURCE  
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int func(void* arg) {
    printf("some function logic\n");
    sleep(1);
    printf("function done\n");
    return 0;
}

// only works on linux
int main() {
    void * stack = malloc(4096);

    int thread_pid = clone(&func, stack+4096, CLONE_SIGHAND|CLONE_FS|CLONE_VM|CLONE_FILES|CLONE_THREAD, NULL);
    printf("thread created\n");

    getchar();
    printf("done\n");
    return 0;
}