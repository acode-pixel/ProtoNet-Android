#ifndef CORE_H
#define CORE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct Packet {
	char Proto[4];
	char Mode[1];
	int IP;

	uint8_t data[1024-9];
};

#endif
