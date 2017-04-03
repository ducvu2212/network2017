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
#include <signal.h>

#define RS_PROMPT_CHAR          "$"
#define BUFFER_SIZE             256
#define RESULT_BUFFER           32768

void error(const char* msg) 
{
	perror(msg);
	exit(1);
}

void welcome_mes(int w)
{
	printf("+----------------------------------------+\n");
	printf("|     Nguyen Duc Vu                      |\n");
	printf("+----------------------------------------+\n");
	printf("|     Project: Remote Shell - Client     |\n");
	printf("+----------------------------------------+\n");
	printf("|     Welcome to my shell                |\n");
	printf("+----------------------------------------+\n\n");
}

int receive_msg(int fd, char* buf) 
{
	int n,total=0;	

	do {
		n = recv(fd,&buf[total],RESULT_BUFFER-total-1,0);
		total += n;
		if(total >= RESULT_BUFFER-1) {
			printf("Buffer overflow.\n");
			exit(1);
		}
	} while(n > 0);

	return total;
}

int main(int argc, char *argv[])
{

//*************************client part**********************************

	int sockfd;
	struct sockaddr_in saddr;
	char command[BUFFER_SIZE], host[100], buf[RESULT_BUFFER];
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
		return 0;
	}

	else {
		printf("Successfully connected\n");
		return sockfd;
	}
		
	//connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr));
	
	welcome_mes(port);

	//data transfer
	while (1) {

		printf("%s ", RS_PROMPT_CHAR);
		bzero(command, sizeof(command));	
		fgets(command, BUFFER_SIZE, stdin);
		char buffer[100], mes[100];
		bzero(buf, sizeof(buf));	

	    //*******************Data Delimiter*****************
	    if (strcmp (mes, "/quit") == 0)
	    {
	    	int count;
			char c;
			while ((count = read(sockfd, &c, sizeof(c))) > 0) {
				shutdown (sockfd, SHUT_RDWR);
				close(sockfd);
			}
	    }

	    //send command to the server
	 	if(send(sockfd, buffer, strlen(buffer), 0) <0)
	 		error("ERROR sending message");

	 	if (strcmp(command, "exit\n") == 0) break;

	    //Receive output from server 
		int total = receive_msg(sockfd, buf);

		if(total>0){
			buf[total]='\0';
			printf("%s\n", buf);	
		}
	}
	close(sockfd);

	return 0;
}