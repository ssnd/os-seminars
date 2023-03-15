#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>


volatile sig_atomic_t counter = 0;
volatile sig_atomic_t done = 0;

void sighandler(int signum) {
    if (signum == SIGINT) {
        counter++;
    } else {
        done = 1;
    }
}

int main () {
    struct sigaction sigint_handler = {
        .sa_flags = SA_RESTART,
        .sa_handler = sighandler,
    };

    sigaction(SIGINT, &sigint_handler, NULL);
    sigaction(SIGTERM, &sigint_handler, NULL);

    printf("%d\n", getpid());
    fflush(stdout);

    while (!done) {
        pause();
    }

    printf("%d\n", counter);
	return 0;

}