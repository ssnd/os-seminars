#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, NULL); 

    while (1) {
        pause();
    }
}