#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	
//*************************server part**********************************

	int sockfd, clen, clientfd;
	struct sockaddr_in saddr, caddr;
	unsigned short port = 8785;
	if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	printf("Error creating socket\n");
}
//create the socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

//Reusing address


	setsockopt(sockfd, SOL_SOCKET,
		SO_REUSEADDR, &(int){1},
		sizeof(int));

//enable nonblocking option
	int fl = fcntl(sockfd, F_GETFL, 0);
	fl |= O_NONBLOCK;
	fcntl(sockfd, F_SETFL, fl);

//bind socket

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	
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

	if ((clientfd=accept(sockfd, (struct sockaddr *) &caddr, &clen)) > 0) {
		int fl = fcntl(clientfd, F_GETFL, 0);
		fl |= O_NONBLOCK;
		fcntl(clientfd, F_SETFL, fl);
	}

	//***********************Data Transfer************************
	while (1) {
		char buffer[100];
	    recv(clientfd, buffer, sizeof(buffer), 0);
	    printf("\nReceive: %s", buffer);
	    printf("\nSend: ");
	    scanf("%s", buffer);
	    
	//*********************Data Delimiter******************
	    if (strcmp (buffer, "/quit") == 0)
	    {
	    	int count;
			char c;
			while ((count = read(sockfd, &c, sizeof(c))) > 0) {
					shutdown (sockfd, SHUT_RDWR);
					close (sockfd);
			}
	    }

	    send(clientfd, buffer, strlen(buffer), 0);
	}

	return 0;
}
