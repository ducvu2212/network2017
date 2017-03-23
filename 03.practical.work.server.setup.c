#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int main(int argc, char *argv[])
{
	int sockfd, clen, clientfd;
	struct sockaddr_in saddr, caddr;
	unsigned short port = 8785;
	if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	printf("Error creating socket\n");
}
//create the socket
sockfd = socket(AF_INET, SOCK_STREAM, 0);


memset(&saddr, 0, sizeof(saddr));
saddr.sin_family = AF_INET;
saddr.sin_addr.s_addr = htonl(INADDR_ANY);
saddr.sin_port = htons(port);

//bind socket
int bind(int sockfd,
const struct sockaddr *bind_addr,
socklen_t addrlen);


if (bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
printf("Error binding\n");
}

//listen for connection requests
if (listen(sockfd, 5) < 0) {
	printf("Error listening\n");
}

listen(sockfd, 10);
clen=sizeof(caddr);

//accept a new socket
if ((clientfd=accept(sockfd, (struct sockaddr *) &caddr, &clen)) < 0) {
	printf("Error accepting connection\n");
}

int client = accept(sockfd, (struct sockaddr_in *) &caddr, &clen);
close (sockfd);
return 0;
}

