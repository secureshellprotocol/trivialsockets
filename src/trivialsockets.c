#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "../include/trivialsockets.h"

#define FUCK(M) perror(M); exit(EXIT_FAILURE);

void _error(char *message){
	perror(message);
	exit(EXIT_FAILURE);
}

Socket *sock_init(int f, int t){
	//legwork
	Socket *s;
	if((s = malloc(sizeof(Socket))) == NULL) { 
		_error("Malloc Failure"); 
	}

	s->family = f;
	s->type = t;
	s->sockfd = -1;

	s->sockfd = socket(s->family, s->type, 0);
	if(s->sockfd == -1){
		_error("Socket failed to initialize!");
	}

	//ckean socket_addr for later binding or connecting
	bzero((char *) &(s->socket_addr), sizeof(s->socket_addr));

	return s;
}

void sock_close(Socket *s){
	close(s->sockfd);
	s->sockfd = -1;

	return;
}

void sock_delete(Socket *s){
	if(s->sockfd != -1){
		sock_close(s);
	}	

	free(s);
	s = NULL;

	return;
}

void sock_bind(Socket *s, short p){
	//socket_addr legwork
	s->socket_addr.sin_family = s->family;
	s->socket_addr.sin_addr.s_addr = INADDR_ANY;
	s->socket_addr.sin_port = htons(p);

	if(bind(s->sockfd, (struct sockaddr *) &(s->socket_addr), sizeof(s->socket_addr)) == -1){
		_error("Failed to bind to port!");
	}

	return;
}

void sock_listen(Socket *s, int c){
	if(listen(s->sockfd, c) == -1){
		_error("Failed to start listening");
	}

	return;
}

Socket *sock_accept(Socket *s){
	struct sockaddr_in client_addr;
	int client_length;

	client_length = sizeof(client_addr);

	Socket *c = sock_init(s->family, s->type);
	if((c->sockfd = accept(s->sockfd, (struct sockaddr *) &client_addr, &client_length)) == -1){
		_error("Failed to accept");
		return NULL;
	}

	return c;
}

void sock_connect(Socket *s, char *h, short p){
	struct hostent *server = gethostbyname(h);
	if(server == NULL){
		fprintf(stderr, "we need an ip!");
	}

	//socket_addr legwork
	s->socket_addr.sin_family = s->family;
	bcopy((char *) &server->h_addr, (char *) &(s->socket_addr.sin_addr.s_addr), server->h_length);
	s->socket_addr.sin_port = htons(p);

	if(connect(s->sockfd, (struct sockaddr *) &(s->socket_addr), sizeof(s->socket_addr)) < 0){
		_error("Connection Refused!");
	}
}
