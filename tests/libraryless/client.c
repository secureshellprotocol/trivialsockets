#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define string char*

void error(const string message){
	perror(message);
	exit(1);
}

int main(int argc, string* argv){
	//--init--
	int sockfd; //to server
	int port;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[255];
	if(argc < 3){
		fprintf(stderr, "usage %s hostname port \n", argv[0]);
		exit(0);
	}
	//prep socket
	port = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("error opening socket");
	}	

	server = gethostbyname(argv[1]);
	if(server == NULL){
		fprintf(stderr, "We need an IP!");
	}
	bzero((string) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((string) server->h_addr, (string) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);
	//--end init--
	//--connect--
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
		error("Connection refused.");
	}
	//loop
	int n;
	while(1){
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, strlen(buffer));
		if(n<0){
			error("Write failed.");
		}
		
		bzero(buffer, 255);
		n = read(sockfd, buffer, 255);
		if(n<0){
			error("Read faile.");
		}
		printf("Server: %s", buffer);

		int i = strncmp("Bye", buffer, 3);
		if(i == 0){ break; }
	}

	close(sockfd);
	return 0;
}
