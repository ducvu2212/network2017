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

#define BUFFER_SIZE 256

void error(const char* msg) 
{
	perror(msg);
	exit(1);    /* Abort */
}

void welcome_mes(int w)
{
	printf("+----------------------------------------+\n");
	printf("|     Nguyen Duc Vu                      |\n");
	printf("+----------------------------------------+\n");
	printf("|     Project: Remote Shell - Server     |\n");
	printf("+----------------------------------------+\n");
	printf("|     Welcome to my shell                |\n");
	printf("+----------------------------------------+\n\n");
}

int main(int argc, char *argv[])
{
	
//*************************server part**********************************

	int sockfd, clen, clientfd;
	struct sockaddr_in saddr, caddr;
	char buffer[BUFFER_SIZE], temp_buf[BUFFER_SIZE];
	ssize_t messageSize;
	char message[256];
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

	//list of clients

	welcome_mes(port); 
	int clientfds[100];
	memset(clientfds, 0, sizeof(clientfds));
	while(1){
		fd_set set;
		FD_ZERO(&set);
		FD_SET(sockfd, &set);
		int maxfd = sockfd;
		for (int i = 0; i < 100; i++) {
            if (clientfds[i] > 0) FD_SET(clientfds[i], &set);
            if (clientfds[i] > maxfd) maxfd = clientfds[i];
        }

//poll, wait and block
		select(maxfd+1, &set, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &set)) {
	        if ((clientfd=accept(sockfd, (struct sockaddr *) &caddr, &clen)) < 0) {
				printf("Error accepting connection\n");
			}
	        //printf("Connection Accepted\n", getpid());
	        // make it nonblocking
	        fl = fcntl(clientfd, F_GETFL, 0);
	        fl |= O_NONBLOCK;
	        fcntl(clientfd, F_SETFL, fl);
	        // add it to the clientfds array
	        for (int i = 0; i < 100; i++) {
	            if (clientfds[i] == 0) {
	                    clientfds[i] = clientfd;
	                    break;
	                }
	            }
			}

		

		//create a child shell on server for client to use
		if(fork() == 0){
			//redirection
			int a = dup2(clientfd, STDOUT_FILENO);
			int b = dup2(clientfd, STDERR_FILENO);
			if(a < 0 || b < 0) error ("ERROR on dup2");
			while(1){
				bzero(buffer, sizeof(buffer));

				//receive

				if(recv(clientfd, buffer, sizeof(buffer), 0) <0)
					error("Receive error");
				


			if (strcmp(buffer, "exit\n") == 0) {
				close(clientfd);
				exit(0);
			}
		


		/* Tokenize command */
				bzero(temp_buf, sizeof(temp_buf));				
				strcpy(temp_buf, buffer);				
				char* p = strtok(temp_buf, " ");    /* token (cmd) */
				
				//if(strcmp(p, LOGIN_CMD)==0) continue;   /* Ignore LOGIN_CMD */
	/* Manage 'cd' */
				if(strcmp(p, "cd")== 0 ) {
					p = strtok(NULL, " ");  /* This is the new path */			
					p[strlen(p)-1] = '\0';  /* replace newline termination char */
					
					/* Change current working directory */					
					if(chdir(p) < 0) perror("error");				
				} 
				else system(buffer);    /* Issue a command */	
			}
			 // for (int i = 0; i < 100; i++) {
			 //            if (clientfds[i] > 0 && FD_ISSET(clientfds[i], &set)) {
			 //                if (read(clientfds[i], message, sizeof(message)) > 0) {
			 //                    printf("client %d says: %s\nserver>", clientfds[i], message);
			 //                }
    //             else {
    //                 // some error. remove it from the "active" fd array
    //                 printf("client %d has disconnected.\n", clientfds[i]);
    //                 clientfds[i] = 0;
    //             }
    //         }
    //     }
    }
        else close (clientfd); /* Close client socket descriptor */
    }

    close(sockfd);
	return 0;
}
