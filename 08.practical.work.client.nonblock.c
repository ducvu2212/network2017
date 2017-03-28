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

int main(int argc, char *argv[])
{

//*************************client part**********************************

	int sockfd, clientfd;
	struct sockaddr_in saddr;
	unsigned short port = 8785;
	if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error creating socket\n");
	}
	//create the socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// get the host info
	int i;
    struct hostent *he;
    struct in_addr **addr_list;

	if ((he = gethostbyname("localhost")) == NULL) {  
	    printf("Unknown host\n");
	}

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	memcpy((char *) &saddr.sin_addr.s_addr, he->h_addr_list[0], he->h_length);
	saddr.sin_port = htons(port);

	//connect

	int connect (int sockfd,
		const struct sockaddr *saddr,
		socklen_t addrlen);

	if (connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
		printf("Cannot connect\n");
	}

	else
		printf("Successfully connected\n");
	//connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr));
	


	//data transfer
	while (1) {

		char buffer[100];
	    printf ("\nSend: ");
	    scanf("%s", buffer);

	    //*******************Data Delimiter*****************
	    if (strcmp (buffer, "/quit") == 0)
	    {
	    	int count;
			char c;
			while ((count = read(clientfd, &c, sizeof(c))) > 0) {
				shutdown (clientfd, SHUT_RDWR);
				close(clientfd);
			}
	    }
	 	send(sockfd, buffer, strlen(buffer), 0);
	    recv(sockfd, buffer, sizeof(buffer), 0);
	    printf("\nReceive: %s", buffer);
	}

	return 0;
}