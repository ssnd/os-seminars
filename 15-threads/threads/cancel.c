#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


static void * func(void*arg) {
    printf("here\n");
    for (;;){};
    sleep(1000);
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, func,NULL);
    sleep(1);

    pthread_cancel(thread);
    printf("thread joining\n");
    pthread_join(thread, NULL);
    printf("done\n");

    return 0;

}