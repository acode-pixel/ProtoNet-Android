#ifndef SERVER_H
#define SERVER_H
#define S_PORT 5657

#include "core.h"
#include "client.h"
#include <sys/ioctl.h>
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

Server* Init(char* inter, char* ip, char* serverName, char Dir[]){
	// Check if Dir is NULL
	assert(Dir != NULL);
	if (access(Dir, R_OK) == -1){
		return NULL;
	}
	
	// alloc server 
	Server* serv = (Server*) malloc(sizeof(Server) + strlen(Dir));
	serv->ServerOpts.socketOpt.keepalive = 1;
	serv->ServerOpts.socketOpt.reuseaddr = 1;
	serv->Socket = socket(AF_INET, SOCK_STREAM, 0);
	// for sockets
	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(S_PORT);
	// Do this to get IP of inter, hate it!
	struct ifreq ifr;
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, inter, IFNAMSIZ-1);
	ioctl(serv->Socket, SIOCGIFADDR, &ifr);
	sockaddr.sin_addr.s_addr = ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr;

	serv->nConn = 0;
	serv->IP = sockaddr.sin_addr.s_addr; // src IP
	if (ip != NULL){
       		inet_pton(AF_INET, ip, (struct in_addr*)&serv->destIP);// dst IP
	}
	strcpy(serv->serverName, serverName);
	memcpy(serv->dir, Dir, strlen(Dir));
	
	setSockOpts(serv->Socket, &serv->ServerOpts.socketOpt, "\x01\x01\x00");

	if (bind(serv->Socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1){
		perror("Server::Init::Error BIND");
		return NULL;
	}

	serv->size = sizeof(*serv) + strlen(serv->dir);
	serv->ServerOpts.sockaddr = (struct sockaddr*)&sockaddr;
	serv->ServerOpts.socklen = sizeof(sockaddr);

	strcpy(serv->client.name, serverName);
	if (ip != NULL){
		serv->client.Socket = connectToNetwork(ip);
		memcpy(&serv->destIP, &sockaddr.sin_addr.s_addr, sizeof(int));
	}
	return serv;
}


#endif
