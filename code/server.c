#include "server.h"

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
	// set fd as nonblocking
	fcntl(serv->Socket, F_SETFL, O_NONBLOCK, 1);

	sockaddr.sin_addr.s_addr = ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr;

	//serv->Clientlist.nClients = 0;
	serv->IP = sockaddr.sin_addr.s_addr; // src IP

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
	if (strlen(ip) > 0){
		serv->client.Socket = connectToNetwork(ip, &serv->client);
		serv->destIP = inet_addr(ip);
	}

	serv->epollInstance = epoll_create(1);
	serv->lepollInstance = epoll_create(MAX_CLIENTS);

	struct epoll_event ev;
	ev.events = EPOLLIN;
	*ev.data.ptr = "LISTEN";
	epoll_ctl(serv->lepollInstance, EPOLL_CTL_ADD, serv->Socket, &ev);
	return serv;
}

int delClient(int fd, Server* serv){
	for (int i = 0; i < MAX_CLIENTS; i++){
		if (serv->Clientlist.clients[i].Socket == fd){
			serv->Clientlist.clients[i].Socket = 0;
			memset(serv->Clientlist.clients[i].name, 0, strlen(serv->Clientlist.clients[i].name));
			serv->Clientlist.clients[i].socketMode = 0;
			serv->nConn -= 1;
			printf("\nDeleted Client %i", fd);
			return 0;
		}
	} 
	return -1;
}

int addClient(int fd, Server* serv){
	struct epoll_event ev;

	if (serv->nConn >= MAX_CLIENTS){
		close(fd);
		printf("\nClient MAX Reached");
		return 0;
	}


	for(int i = 0; i <= MAX_CLIENTS; i++){
		if (i > MAX_CLIENTS-1){
			return -1;
		}

		else if (serv->Clientlist.clients[i].Socket == 0){
			serv->Clientlist.clients[i].Socket = fd;
			serv->nConn += 1;
			ev.events = EPOLLIN | EPOLLOUT;
			ev.data.fd = fd;
			epoll_ctl(serv->epollInstance, EPOLL_ADD, fd, &ev);
			printf("\nAdded %i to kqueue", fd);
			return 0;
		}
	}

	return -1;
}

int checkSockets(Server* serv){
	int nevents = epoll_wait(serv->epollInstance, serv->Events, MAX_CLIENTS, 1500);
	printf("\n%i", nevents);
	if (nevents == 0){
		return 0;
	}
	// Should redesign loop if MAX_CLIENTS is larger
	for (int i = 0; i < MAX_EVENTS; i++){

		if (serv->Events[i].events == EPOLLIN){
			printf("\n%s", serv->Events[i].data.ptr);
			printf("\nreading fd: %lu", serv->Events[i].data.fd);
			Packet* buf = (Packet*) malloc(sizeof(Packet));
			if (recv(serv->Events[i].data.fd, buf, sizeof(*buf), 0) == -1){
				perror("read Failed:");
				return errno;
			}

			struct in_addr addr;

			printf("\nProtocol: %s", buf->Proto);
			printf("\nDatalen: %i", buf->datalen);
			printf("\nMode: %x", *buf->Mode);
			addr.s_addr = buf->IP;
			printf("\nDst IP: %s", inet_ntoa(addr));
			read(serv->Events[i].data.fd, buf->data, buf->datalen);
			printf("\n%s", ((struct BROD*)buf->data)->fileReq);

			delClient(serv->Events[i].data.fd, serv);
			close(serv->Events[i].data.fd);

		
		}
	}

	return 0;
}


int ServerListen(Server* serv){
	struct epoll_event ev;

	int nSockets = epoll_wait(serv->lepollInstance, &ev, 1, 1500);
	printf("\nListening Events: %i", nSockets);

	if (ev.data.event == EPOLLIN){
		int fd = accept(ev.ident, serv->ServerOpts.sockaddr, &serv->ServerOpts.socklen);	
		return fd;
	}
	return 0;

}
