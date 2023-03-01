#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct args {
    int a;
    int b;
};

struct ret {
    int c;
};

struct ret* add(struct args* args) {
    printf("thread: add args a=%d, b=%d\n", args->a, args->b);

    struct ret * myret= malloc(sizeof(struct ret));
    myret->c = args->a + args->b;
    return myret;
}

int main() {
    pthread_t thread;
    struct args myargs = {.a=42, .b=100500};

    struct ret *myret;
    pthread_create(&thread, NULL, (void* (*) (void*))add, (void*) &myargs);
    pthread_join(thread, (void**) &myret);

    printf("done, ret_value=%d\n", myret->c);
    free(myret);
    return 0;
}