#ifndef SERVER_H
#define SERVER_H
#define S_PORT 5657

#include "core.h"
#include "client.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <net/if.h>

typedef struct serverOpts {
	SocketOpt socketOpt;
	struct sockaddr* sockaddr;
	socklen_t socklen;
} serverOpts;

typedef struct Server {
	int Socket;
	int nConn;
	uint32_t IP;
	uint32_t destIP;
	size_t size;
	char serverName[12];
	serverOpts ServerOpts;
	Client client;
	char dir[];
} Server;

Server* Init(char* inter, char* ip, char* serverName, char Dir[]);


#endif
