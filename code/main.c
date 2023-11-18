#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include "client.h"
#include "server.h"
#include "core.h"

int main(int argc, char* argv[]){
	
	if (strcmp(argv[1], "client")==0){
		Client* test1 = NULL;
		test1 = Cl_Init(argv[2], argv[3]);
		printf("Connecting to %s", argv[4]);
		if (connectToNetwork(argv[4], test1) == -1){
			perror("Failed to connect to client");
		}

		sendPck(test1->Socket, inet_addr(argv[4]), "\x01", "test");

		return 0;
	}
	
	if (strcmp(argv[1], "serv") == 0){
		Server* test2 = NULL;
		test2 = (Server*) Init(argv[2], argv[3], "Test", "/Users/donaldgutierrez");

		assert(test2 != NULL);

		assert(listen(test2->Socket, 10)==0);
		int fd = accept(test2->Socket, test2->ServerOpts.sockaddr, &test2->ServerOpts.socklen);

		printf("Starting Server");
		
		Packet buf;
		read(fd, &buf, sizeof(buf));
		printf("\n%s", buf.data);

	}

	return 0;
}
