#ifndef SERVER_H
#define SERVER_H
#define S_PORT 5657

#include "core.h"
#include <unistd.h>
#include <arpa/inet.h>

struct SocketOpt {
	int reuseaddr;
	int keepalive;
	int dontroute;
	int debug;
};

typedef struct serverOpts {
	struct SocketOpt socketOpt;
} serverOpts;

typedef struct Server {
	int Socket;
	int nConn;
	uint32_t IP;
	char serverName[12];
	serverOpts ServerOpts;
	char dir[];
} Server;

Server* Init( char ip, char serverName, char Dir){
	assert(Dir != NULL);

	if (access(Dir, R_OK) == -1){
		return NULL;
	}

	Server* serv = (Server*) malloc(sizeof(Server) + sizeof(*Dir));
	serv->ServerOpts.socketOpt.keepalive = 1;
	serv->Socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(S_PORT);
	inet_aton(&ip, &sockaddr.sin_addr);

	serv->nConn = 0;
	memcpy(&serv->IP, &sockaddr.sin_addr.s_addr, sizeof(int));
	strcpy(serv->serverName, &serverName);
	memcpy(serv->dir, &Dir, strlen(&Dir));
	
	assert(setsockopt(serv->Socket, SOL_SOCKET, SO_KEEPALIVE, &serv->ServerOpts.socketOpt.keepalive, sizeof(int)) == 0);

	if (bind(serv->Socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1){
		printf("Server::Init::Error %i", errno);
		return NULL;
	}

	return serv;

}
#endif
