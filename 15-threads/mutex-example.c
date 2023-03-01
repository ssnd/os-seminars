#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>

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

int write_to_server(struct server* srv, char data) {
    if (srv->len + 1 > srv->cap) {
        return -1;
    }

    srv->buf[srv->len] = data;
    srv->len++;

    return 0;
}

void dump_buff(struct server* ptr) {
    printf("server data dump: %s\n", ptr->buf);
}


static const size_t n_threads = 10;
static const size_t tries = 3;
static const size_t msg_len = 32;

struct arg {
    int no;
    struct server* srv;
    pthread_mutex_t* mutex;
};

void* thread_func(struct arg* arg) {
    for (size_t j = 0; j < tries; ++j) {
        pthread_mutex_lock(arg->mutex);
        // critical section start
        for (size_t i = 0; i < msg_len; ++i) { // 1
            write_to_server(arg->srv, '0'+arg->no);
            sched_yield();
        }
        // critical section end
        pthread_mutex_unlock(arg->mutex);
    }
    return NULL;
}


int main(int argc, char** argv) {
    struct server* srv = init_server(n_threads*tries*msg_len);

    pthread_t* arr = malloc(sizeof(pthread_t)*n_threads);
    struct arg* args = malloc(sizeof(struct arg)*n_threads);

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for (size_t i = 0; i < n_threads; ++i) {
        args[i].no = i;
        args[i].srv = srv;
        args[i].mutex = &mutex;
        pthread_create(&arr[i], NULL, (void* (*)(void*)) thread_func, (void*)&args[i]);
    }

    for (size_t i = 0; i < n_threads; ++i) {
        pthread_join(arr[i], NULL);
    }

    free(arr); free(args);
    pthread_mutex_destroy(&mutex);
    dump_buff(srv);

    destroy_server(srv);
}