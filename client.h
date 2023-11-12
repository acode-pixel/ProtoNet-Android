#ifndef CLIENT_H
#define CLIENT_H

#include "./core.h"
#define C_PORT 8888

typedef struct Client {
	char* name;
	int Socket;
} Client;

int connectToNetwork(int IP){
	int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(C_PORT);
	sockaddr.sin_addr.s_addr = htonl((uint32_t)IP);

	printf("%i %p %lu", tcpSocket, &sockaddr, sizeof(sockaddr));

	if ((bind(tcpSocket, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) == -1){
		exit(errno);
	}
	

}

#endif
