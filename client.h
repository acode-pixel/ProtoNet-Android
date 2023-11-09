#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "./core.h"
#define PORT 5656

int connectToNetwork(uint32_t IP){
	int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(PORT);
	sockaddr.sin_addr.s_addr = htonl(IP);

	assert((bind(tcpSocket, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) == 0);

}

#endif
