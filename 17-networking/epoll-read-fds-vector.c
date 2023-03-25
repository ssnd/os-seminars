#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int fd;
    size_t count;
    bool done;
} data_t;


static void make_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags|O_NONBLOCK);
}

static data_t * register_event(int epoll_fd, int fd) {
    make_non_blocking(fd);

    data_t * data = calloc(1, sizeof(*data));

    data->fd = fd;

    struct epoll_event event_ready_read;
    event_ready_read.events = EPOLLIN;
    event_ready_read.data.ptr = data;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event_ready_read);

    return data;
}


static void process_pair_ready_read(data_t * data) {
    char buff[4096];

    ssize_t cnt = read(data->fd,  buff, sizeof(buff));
    if (cnt > 0) {
        data->count += cnt;
    }
    else if(0==cnt) {
        data->done = true;
        close(data->fd);
    }
}


static void process_event(struct epoll_event * event, size_t * files_left) {
    const uint32_t mask = event->events;
    data_t * data = event->data.ptr;

    if (mask & EPOLLIN) {
        process_pair_ready_read(data);
    }

    if (data->done) {
        *files_left-=1;
    }
}

extern size_t read_data_and_count(size_t N, int in[]) {
    data_t **entries = calloc(N, sizeof(*entries));

    int epoll_fd = epoll_create1(0);

    for (size_t i = 0; i < N; ++i) {
        entries[i] = register_event(epoll_fd, in[i]);
    }

    size_t files_left = N;
    struct epoll_event pending[10000];


    while (files_left > 0) {
        int n = epoll_wait(epoll_fd, pending, 10000, -1);
        for (size_t i = 0; i < n; ++i){
            process_event(&pending[i], &files_left);
        }
    }

    close(epoll_fd);

    size_t result = 0;

    for (size_t i = 0; i < N; ++i) {
        result+= entries[i]->count;

    }

    free(entries);

    return result;
}