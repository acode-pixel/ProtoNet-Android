#ifndef CORE_H
#define CORE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

typedef struct Packet {
	char Proto[4];
	uint32_t IP;
	char Mode[1];

	uint8_t data[1024-9];
} Packet;

typedef struct SocketOpt {
	char reuseaddr;
	char keepalive;
	char dontroute;
} SocketOpt;

int setSockOpts(int sock, SocketOpt* so, char opts[]){
	so->reuseaddr = opts[0];
	so->keepalive = opts[1];
	so->dontroute = opts[2];

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opts[0], sizeof(int));
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &opts[1], sizeof(int));
	setsockopt(sock, SOL_SOCKET, SO_DONTROUTE, &opts[2], sizeof(int));
	
	return 0;
}

#endif
