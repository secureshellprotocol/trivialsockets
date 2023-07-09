//cpp header for sockets

#ifndef _SOCKET_CLASS_

typedef struct trivialsocket_t{
	int family;
	int type;
	int sockfd;
	struct sockaddr_in socket_addr;

} Socket;

Socket *sock_init(int f, int t);
void sock_close(Socket *s);
void sock_delete(Socket *s);
void sock_bind(Socket *s, short p);
void sock_listen(Socket *s, int c);
Socket *sock_accept(Socket *s);
void sock_connect(Socket *s, char *h, short p);

#define _SOCKET_CLASS_
#endif
