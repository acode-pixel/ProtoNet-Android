#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "client.h"
#include "server.h"
#include "core.h"

int main(int argc, char* argv[]){
	
	Packet *test = NULL;
	test = (Packet*) malloc(sizeof(Packet));
	uint8_t* data = "BROD\xff\xff\xff\xff\x0a";
	memset(test, 0, sizeof(*test));
	memcpy(test->Proto, (char*)"SPTP", sizeof(int));
	memcpy(test->Mode, (char*)"\x01", sizeof(char));
	memcpy(&test->IP, (uint32_t*)"\xa3\xff\x00\x24", sizeof(uint32_t));
	memcpy(test->data, data, sizeof(data)+1);

	FILE* f = fopen("mem.bin", "wb");
	fwrite(test, 1, 1024, f);
	fclose(f);

	//connectToNetwork((int)"\x7f\x00\x00\x01");
	Server* test2 = NULL;
	test2 = (Server*) Init("127.0.0.1", "Test", "/Users/donaldgutierrez");

	assert(test2 != NULL);

	FILE* a = fopen("Server.bin", "wb");
	fwrite(test2, 1, sizeof(*test2) + strlen(test2->dir), a);
	fclose(a);

	return 0;
}
