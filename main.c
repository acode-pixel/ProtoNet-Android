#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "client.h"
#include "server.h"
#include "core.h"

int main(int argc, char* argv[]){
	
	if (strcmp(argv[1], "client")==0){
		printf("Connecting to %s", argv[2]);
		if (connectToNetwork(argv[2]) == -1){
			perror("Failed to connect to client");
		}
		return 0;
	}
	
	if (strcmp(argv[1], "serv") == 0){
		Server* test2 = NULL;
		test2 = (Server*) Init(argv[2], argv[3], "Test", "/Users/donaldgutierrez");

		assert(test2 != NULL);

		assert(listen(test2->Socket, 10)==0);
		assert(accept(test2->Socket, test2->ServerOpts.sockaddr, &test2->ServerOpts.socklen)>-1);

		printf("Starting Server");
	}

	return 0;
}
