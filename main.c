#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "client.h"
#include "core.h"

int main(int argc, char* argv[]){
	struct Packet {
		char Proto[4];
		char Mode[1];
		int IP;

		uint8_t data[1024-9];
	};
	
	struct Packet *test = NULL;
	test = (struct Packet*) malloc(sizeof(struct Packet));
	uint8_t* data = "BROD\xff\xff\xff\xff\x0a";
	memset(test, 0, sizeof(*test));
	memcpy(test->Proto, (char*)"SPTP", sizeof(int));
	memcpy(test->Mode, (char*)"\x01", sizeof(char));
	memcpy(&test->IP, (int*)"\xa3\xff\x00\x24", sizeof(int));
	memcpy(test->data, data, sizeof(data)+1);

	FILE* f = fopen("mem.bin", "wb");
	fwrite(test, 1, 1024, f);
	fclose(f);

	connectToNetwork((uint32_t)"\x80");

	return 0;
}
