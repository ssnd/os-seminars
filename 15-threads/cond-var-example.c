#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdatomic.h>

struct server {
    char* buf;
    size_t len;
    size_t cap;
};

struct server* init_server(size_t cap) {
    struct server* ptr = malloc(sizeof(struct server));
    ptr->buf = calloc(cap, sizeof(char));
    ptr->len = 0;
    ptr->cap = cap;
    return ptr;
}

void destroy_server(struct server* srv) {
    free(srv->buf);
    free(srv);
}

int write_to_server(struct server* srv, char data ) {
    if (srv->len + 1> srv->cap) {
        return -1;
    }

    srv->buf[srv->len] = data;
    srv->len++;

    return 0;
}

static const size_t n_threads = 10;
static const size_t tries = 3;
static const size_t msg_len = 3;

struct arg {
    int no;
    struct server* srv;
    uint8_t* full;
    pthread_mutex_t* mutex;
    pthread_cond_t* cv;
};

void* writer_func(struct arg* arg) {
    pthread_mutex_lock(arg->mutex);
    for (size_t i = 0; i < msg_len; ++i) {
        write_to_server(arg->srv, '0'+i);
        sleep(1);
    }
 
    *(arg->full) = 1;
    pthread_cond_signal(arg->cv);
    pthread_mutex_unlock(arg->mutex);

    return NULL;
}

void* reader_func(struct arg* arg) {
    pthread_mutex_lock(arg->mutex);

    while (*(arg->full) != 1) {
        pthread_cond_wait(arg->cv, arg->mutex);
    }

    printf("finally reading the buffer\n");
    pthread_mutex_unlock(arg->mutex);
    return NULL;
}

int main(int argc, char** argv) {
    struct server* srv = init_server(msg_len);
    pthread_t reader, writer;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
    uint8_t full;

    struct arg arg = {.no = 1, .full = &full, .srv = srv, .cv=&cv, .mutex=&mutex};

    pthread_create(&reader, NULL, (void* (*)(void*)) reader_func, (void*)&arg);
    pthread_create(&writer, NULL, (void* (*)(void*)) writer_func, (void*)&arg);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cv);
    destroy_server(srv);
}