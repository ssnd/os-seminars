#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static void * thread_func(void * arg) {
    int val;

    if (scanf("%d", &val) != 1) {
        return NULL;
    }

    pthread_t next_thread;
    int s;
    s = pthread_create(&next_thread, NULL, thread_func, 0);
    if (s != 0) {
        perror("pthread_create");
        exit(1);
    }

    s = pthread_join(next_thread, NULL);
    if (s != 0) {
        perror("pthread_join");
        exit(1);
    }
    printf("%d ", val);
    return NULL;
}


int main() {
    pthread_t thread;
    int s;
    s = pthread_create(&thread, NULL, thread_func, 0);
    if (s != 0) {
        perror("pthread_create");
        exit(1);
    }

    s = pthread_join(thread, NULL);
    if (s != 0) {
        perror("pthread_join");
        exit(1);
    }
    return 0;
}