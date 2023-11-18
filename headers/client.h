#ifndef CLIENT_H
#define CLIENT_H

#include "core.h"
#define C_PORT 5657

typedef struct Client {
	int Socket;
	char name[12];
} Client;

Client* Cl_Init(char* inter, char name[]);

int connectToNetwork(char* IP, Client* cli);
int sendPck(int fd, uint32_t IP, char* Mode, char data[]);
#endif
