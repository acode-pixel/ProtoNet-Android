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

typedef struct Packet {
	char Proto[4];
	uint32_t IP;
	char Mode[1];

	uint8_t data[1024-9];
} Packet;

#endif
