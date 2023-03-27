#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>

static const int N = 2;
static const int bufSize = 4096;

int main() {
    int input_fds[N];
    const int ports[N] = {8080, 8081};
    int s;

    for (int i = 0; i < N; ++i) {
        input_fds[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == input_fds[i]) {
            perror("socket");
            return 1;
        }

        struct in_addr addr = {
            .s_addr = inet_addr("127.0.0.1")
        };

        struct sockaddr_in servaddr = {
            .sin_family = AF_INET,
            .sin_port = htons(ports[i]),
            .sin_addr = addr,
        };

        s = connect(input_fds[i],(struct sockaddr*) &servaddr, sizeof(servaddr));
        if (s != 0) {
            perror("connect");
            return 1;
        }

        fcntl(input_fds[i], F_SETFL, fcntl(input_fds[i], F_GETFL) | O_NONBLOCK);
        printf("connection ok\n");
    }

    int open = N;
    while (open > 0){
        for (size_t i = 0; i < N; i++) {
            int bytes_read;
            if (-1 == input_fds[i]) {
                continue;
            }

            char buf[bufSize];
            if ((bytes_read = read(input_fds[i], buf, sizeof(buf))) < 0) {
                // EAGAIN (when using non-blocking I/O) is raised if there's no available data.
                // EINTR means the `read` syscall was interrupted by a signal.
                if (errno == EAGAIN || errno == EINTR) { // this is fine 
                    continue;
                } else {
                    perror("read");
                    continue;
                }
            }

            // read data from socket
            if (bytes_read > 0) {
                printf("bytes read from fd=%ld\n", i);
                continue;
            }

            // EOF received, shutdown and cleanup
            if (bytes_read == 0) {
                shutdown(input_fds[i], O_RDWR);
                open--;
                close(input_fds[i]);
                input_fds[i] = -1;
                printf("eof fd=%ld\n", i);
                continue;
            }

        }
    }
    return 0;
}