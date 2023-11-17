#ifndef CLIENT_H
#define CLIENT_H

#include "./core.h"
#define C_PORT 5657

typedef struct Client {
	int Socket;
	char name[12];
} Client;

int connectToNetwork(char IP[]){
	int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(C_PORT);
	inet_aton(IP, &sockaddr.sin_addr);

	//printf("%i %p %lu", tcpSocket, &sockaddr, sizeof(sockaddr));
	
	if (connect(tcpSocket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1){
		return -1;
	}

	return tcpSocket;

}

#endif
