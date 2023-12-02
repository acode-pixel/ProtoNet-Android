#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
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

		makeFileReq(test1->Socket, argv[4], argv[5]);
		close(test1->Socket);

		return 0;
	}

	if (strcmp(argv[1], "serv") == 0){
		Server* test2 = NULL;
		test2 = Init(argv[2], argv[3], argv[4], argv[5]);

		assert(test2 != NULL);

		assert(listen(test2->Socket, 10)==0);

		while (true){
			int fd = ServerListen(test2);
			printf("\nfd: %i", fd);
			printf("\nnConn: %i", test2->nConn);
			if (fd > 0){
				addClient(fd, test2);
			}
			checkSockets(test2);

			/*printf("\n%i", test2->Socket);
			printf("\n%i", fd);

			if (fd <= 0){
				continue;
			}
			
			struct in_addr addr;
		
			Packet* buf = (Packet*) malloc(sizeof(Packet));
			if (read(fd, buf, sizeof(*buf)) == -1){
				perror("read failed:");
				exit(errno);
			}
			printf("\nProtocol: %s", buf->Proto);
			printf("\nDatalen: %i", buf->datalen);
			printf("\nMode: %x", *buf->Mode);
			addr.s_addr = buf->IP;
			printf("\nDst IP: %s", inet_ntoa(addr));
			read(fd, buf->data, buf->datalen);
			printf("\n%s", ((struct BROD*)buf->data)->fileReq);
			//close(fd);	*/
		}
	}
	return 0;
}
