#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static void handler(int signum, siginfo_t* info, void* context) {
    static const char msg[] = "SIGALRM caught\n";
    write(STDOUT_FILENO, msg, sizeof(msg)-1);
}

int main() {
    struct sigaction sigpipe_handler = {
        .sa_flags = SA_SIGINFO | SA_RESTART,
        .sa_handler = (void*) handler,
    };

    sigaction(SIGALRM, &sigpipe_handler, NULL);
    alarm(1);
    pause();
}
