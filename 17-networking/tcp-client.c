#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

int main(int argc, char * argv[]) {
  int port = strtol(argv[2], NULL, 10);
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (-1 == sock_fd) {
    perror("socket");
    exit(1);
  }

  sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_addr =  inet_addr(argv[1]),
    .sin_port = htons(port)
  };

  if (connect(sock_fd,(struct sockaddr*)&addr, sizeof(struct sockaddr_in))) {
    close(sock_fd);
    perror("connect");
    exit(1);
  }
  int send, recv;
  while (scanf("%d", &send) != EOF) {
    if (write(sock_fd, &send, sizeof(int)) <=0) break;
    if (read(sock_fd, &recv, sizeof(int)) <=0 ) break;
    printf("%d\n", recv);
  }

  close(sock_fd);
  shutdown(sock_fd, 2);
  return 0;
}