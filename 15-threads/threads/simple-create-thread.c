#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* func(void* arg) {
    printf("func starting...\n");
    sleep(1);
    printf("func done\n");
    return NULL;
} 

int main() {
    pthread_t thread;
    printf("thread created\n");
    int s = pthread_create(&thread, NULL, func, 0);
    if (s != 0) {
        perror("pthread_create");
        return 1;
    }

    printf("waiting for the thread finish its thing\n");
    s = pthread_join(thread, NULL);
    if (s != 0) {
        perror("pthread_join");
        return 1;
    }
    printf("done!\n");

    return 0;
}