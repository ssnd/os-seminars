#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static void handler(int signum, siginfo_t* info, void* context) {
    static const char msg[] = "SIGCHLD caught\n";
    write(STDOUT_FILENO, msg, sizeof(msg)-1);
}

int main() {
    struct sigaction sigchld_handler = {
        .sa_flags = SA_SIGINFO | SA_RESTART,
        .sa_handler = (void*) handler,
    };

    sigaction(SIGCHLD, &sigchld_handler, NULL);
    pid_t pid;
    if (-1 == (pid = fork())) {
        perror("fork");
        return 1;
    }

    if (0 == pid) {
        printf("ping from child, pid=%d\n", getpid());
        sleep(1);
        printf("я усталь\n");
        exit(1);
    } else {
        pause();
    }
}