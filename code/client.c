#include "client.h"

Client* Cl_Init(char* inter, char name[]){
	Client* cli = (Client*) malloc(sizeof(Client));
	cli->Socket = socket(AF_INET, SOCK_STREAM, 0);
	struct in_addr addr;
	addr.s_addr = getInterIP(cli->Socket, inter);
	if (name == NULL){
		strcpy(cli->name, (char*)(inet_ntoa(addr)));
	} 
	
	else {strcpy(cli->name, name);}

	return cli;
}

int connectToNetwork(char* IP, Client* cli){
	int tcpSocket = (cli->Socket == 0) ? socket(AF_INET, SOCK_STREAM, 0) : cli->Socket;

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(C_PORT);
	inet_aton(IP, &sockaddr.sin_addr);

	//printf("%i %p %lu", tcpSocket, &sockaddr, sizeof(sockaddr));
	
	if (connect(tcpSocket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1){
		return -1;
	}
	
	return tcpSocket;

}

int sendPck(int fd, uint32_t IP, char* Mode, char data[]){
	Packet* pck = (Packet*) malloc(sizeof(Packet));
	memcpy(pck->Proto, "SPTP", 4);
	memcpy(&pck->IP, &IP, sizeof(IP));
	memcpy(pck->Mode, Mode, 1);
	if (sizeof(*data) > 1015){
		errno = 84;
		perror("Pck creation error");
		return -1;
	}
	memcpy(pck->data, data, strlen(data));
	
	if (send(fd, pck, sizeof(*pck), 0) == -1){
		perror("Falied to send Pck");
		return -1;
	}

	free(pck);
	return 0;
}
