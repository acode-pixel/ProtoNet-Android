#ifndef SERVER_H
#define SERVER_H
#define S_PORT 5657
#define MAX_CLIENTS 10
#define MAX_EVENTS 10

#include "core.h"
#include "client.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/epoll.h>
#include <sys/time.h>

typedef struct serverOpts {
	SocketOpt socketOpt;
	struct sockaddr* sockaddr;
	socklen_t socklen;
} serverOpts;

typedef struct clientList {
	Client clients[MAX_CLIENTS];
} clientList;

typedef struct Server {
	int Socket;		/* Socket */
	int epoll;
	int lepoll;
	int nConn;
	uint32_t IP;		/* host IP */
	uint32_t destIP;	/* Destination IP */
	size_t size;		/* Server struct Size */
	char serverName[12];	/* Server Name */ 
	serverOpts ServerOpts;	/* Server Options */
	struct epoll_event Events[MAX_CLIENTS];
	Client client;		/* for client-server hybrid */
	clientList Clientlist;
	char dir[];	/* Server Dir */
} Server;

Server* Init(char* inter, char* ip, char* serverName, char Dir[]);
int addClient(int fd, Server* serv);
int checkSockets(Server* serv);
int ServerListen(Server* serv);


#endif
