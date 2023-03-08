#include <signal.h>
#include <unistd.h>

static void handler(int signum) {
    static const char msg[] = "SIGINT caught\n";
    write(STDOUT_FILENO, msg, sizeof(msg)-1);
}

int main() {
    struct sigaction setup = {
        .sa_handler = handler,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGINT, &setup, NULL);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    while (1) {
        sigprocmask(SIG_BLOCK, &mask, NULL);
        sleep(10);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
    }
}