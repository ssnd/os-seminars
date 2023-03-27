#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>


static const char resp[] = "sleeping beauty says hi";

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("usage: ./server <port> <sleep (seconds)>\n");
        return 1;
    }

    int port = atoi(argv[1]);
    int sleep_seconds = atoi(argv[2]);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0); // man socket
    if (-1 == socket_fd) {
        perror("socket");
        return 1;
    }
    printf("socket created successfully\n");

    struct in_addr addr = {
        .s_addr = inet_addr("127.0.0.1")
    };

    const struct sockaddr_in host_addr = {
        .sin_port = htons(port),
        .sin_family = AF_INET,
        .sin_addr = addr,
    };
    int addr_len = sizeof(host_addr);

    int s;
    s = bind(socket_fd, (const struct sockaddr *)&host_addr,addr_len);
    if (s != 0) {
        perror("bind");
        return 1;
    }
    printf("socket bound successful\n");

    s = listen(socket_fd, SOMAXCONN);
    if (s != 0) {
        perror("listen");
        return 1;
    }
    printf("server listening for connections...\n");


    for (;;) {
        int new_socket_fd = accept(
            socket_fd,
            (struct sockaddr*)&host_addr,
            (socklen_t*)&addr_len
        );

        if (new_socket_fd < 0) {
            perror("accept");
            continue;
        }

        sleep(sleep_seconds); // that's why we call it a sleepy server

        write(new_socket_fd, resp, strlen(resp));
        printf("connection accepted\n");
        shutdown(new_socket_fd, SHUT_RDWR);
        close(new_socket_fd);
    }

    return 0;

}