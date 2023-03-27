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
        printf("connection ok\n");
    }

    for (size_t i = 0; i < N; i++) {
        char buf[bufSize];
        int bytes_read = read(input_fds[i], buf, sizeof(buf));
        if (bytes_read > 0) {
            printf("bytes read from %ld\n", i);
        } else if (bytes_read == 0) { // eof
            shutdown(input_fds[i], O_RDWR);
            close(input_fds[i]);
            printf("eof\n");
        } else {
            perror("read");
            continue;
        }
    }
    return 0;
}