#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../include/trivialsockets.h"

int main(){
	Socket *server = sock_init(AF_INET, SOCK_STREAM);
	char buffer[255];

	sock_bind(server, 12345);	//listening on port 12345
	sock_listen(server, 1);		//1 client
	
	Socket *client = sock_accept(server);

	int n, i;
	while(1){
		//note to james
		//make a sanity check where the server cant write on its own  file descriuptot
		bzero(buffer, 255);
		n = read(client->sockfd, buffer, 255);
		if(n == -1){ printf("oops"); exit(EXIT_FAILURE); }
		printf("client: %s\n",buffer);

		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(client->sockfd, buffer, strlen(buffer));
		if(n == -1){ printf("oops"); exit(EXIT_FAILURE); }
		i = strncmp("Bye", buffer, 3);
		if(i == 0) {break;}
	}

	sock_close(client);
	sock_close(server);
	
	return 0;
}
