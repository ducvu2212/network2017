#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>


int main(int argc, char *argv[])
{
    char hostname[100];


    int i;
    struct hostent *he;
    struct in_addr **addr_list;

    if (argc != 2) {
        printf ("Please enter a hostname: ");
        scanf("%s", hostname);
    }
    else {
        // transfer from argv[1] to hostname
        strcpy(hostname, argv[1]);
    }

    if ((he = gethostbyname(hostname)) == NULL) {  // get the host info
        herror("hostname");
        return 2;
    }




    // print information about this host:
    printf("Official name is: %s\n", he->h_name);
    printf("    IP addresses: ");
    addr_list = (struct in_addr **)he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) {
        printf("%s ", inet_ntoa(*addr_list[i]));
    }
    printf("\n");

    return 0;
}

