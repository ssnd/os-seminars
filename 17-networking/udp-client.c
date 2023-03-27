#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
	if (argc != 2) {
		printf("usage: ./udp-client <port>\n");
		exit(1);
	}

	int port_num = strtol(argv[1], NULL, 10);
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sock) {
		perror("socket");
		exit(1);
	}

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_addr = inet_addr("127.0.0.1"),
		.sin_port = htons(port_num)
	};

	int send_num, recv_num;
	while (scanf("%d", &send_num) != EOF) {
		sendto(sock, &send_num, sizeof(send_num), 0, (const struct sockaddr*)&addr, sizeof(addr));
		recvfrom(sock, &recv_num, sizeof(recv_num), 0, NULL, NULL);
		printf("%d\n", recv_num);
	}
	close(sock);
}
