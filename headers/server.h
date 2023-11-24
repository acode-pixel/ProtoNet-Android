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
	int Socket;		/* Socket */
	int nConn;		/* number of clients connected */
	uint32_t IP;		/* host IP */
	uint32_t destIP;	/* Destination IP */
	size_t size;		/* Server struct Size */
	char serverName[12];	/* Server Name */
	serverOpts ServerOpts;	/* Server Options */
	Client client;		/* for client-server hybrid */
	char dir[];		/* Server Dir */
} Server;

Server* Init(char* inter, char* ip, char* serverName, char Dir[]);


#endif
