#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


static void * func(void * arg) {
    int * a = malloc(sizeof(int));

    printf("hello \n");
    *a = 42;
    return (void*)a;
}

int main() {
    pthread_t thread;
    int *return_value;

    pthread_create(&thread, NULL, func, (void *) &return_value);
    pthread_join(thread, (void**)&return_value);

    printf("return_value=%d\n", (int) *return_value);

    return 0;
}