#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t done = 0;
volatile sig_atomic_t cnt = 0;

void handler(int signum, siginfo_t* info, void* context) {
    static const char msg[] = "SIGINT caught\n";
    write(STDOUT_FILENO, msg, sizeof(msg)-1);
}

int main() {
    const struct sigaction sigpipe_handler = {
        .sa_flags = SA_SIGINFO | SA_RESTART,
        .sa_handler = (void*) handler,
    };

    sigaction(SIGINT, &sigpipe_handler, NULL);

    while (!done) {
        sleep(10);
    }

    printf("cleaning up...\n");
}