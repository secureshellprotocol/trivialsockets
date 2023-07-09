#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define string char*

void error(const string message){
	perror(message);
	exit(1);
}

/*
 *	arg1 - file name
 *	arg2 - port
 */
int main(int argc, string* argv){
	if(argc != 2) {
		error("Please provide a port.");
	}
	
	int sockfd;	//serbian
	int newsockfd; 	//client
	int port;	//port of serbian
	char buffer[255];

	struct sockaddr_in serbian_addr;
	
	socklen_t client_length;
	struct sockaddr_in client_addr;

	//--init--
	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0); //ipv4, tcp
	if(sockfd < 0){
		error("Error initializing socket");
	}
	
	//clean struct sockaddr_in
	bzero((string) &serbian_addr, sizeof(serbian_addr));
	port = atoi(argv[1]);
	//prep struct
	serbian_addr.sin_family = AF_INET;
	serbian_addr.sin_addr.s_addr = INADDR_ANY;
	serbian_addr.sin_port = htons(port);
	
	//--end init--
	//--binding--
	if(bind(sockfd, (struct sockaddr*) &serbian_addr, sizeof(serbian_addr)) < 0){
		error("Bingding has le shit itself");
	}
	//--end binding00
	//--listening and accepting--
	listen(sockfd, 5); //5 is max amt of clients at a time!
	client_length = sizeof(client_addr);

	//every time we get a connection, a new file descriptor is made for that connection.
	newsockfd = accept(sockfd, (struct sockaddr*) &client_addr, &client_length);
	if(newsockfd < 0){
		error("Error upon accepting connection");
	}
	//--end listening and accepting--
	//start communication loop
	fprintf(stdout, "Server is starterd");	
	int n;
	while(1){
		bzero(buffer, 255); //continuously clean buffer
		n = read(newsockfd, buffer, 255);
		if(n == -1){
			error("Error on reading!");
		}
		printf("client: %s\n", buffer); //print the clients message
		
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(newsockfd, buffer, strlen(buffer));
		if(n == -1){
			error("Error on writing!");
		}
		int i = strncmp("Bye", buffer, 3);
		if(i == 0){ break; }

	}
	//--end communication loop--
	//--clean--
	close(newsockfd);
	close(sockfd);
	return 0;
}
