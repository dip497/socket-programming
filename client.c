/*
Command writing format:

filename server_inpaddress portno
argv[0] filename
argv[1] server_ipaddress
argv[2] portno

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
// Defines the hostent structure - used to store info about a given host eg., host name and ipv4 address.

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) 
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]); // Convert string to number.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]); // Inbuilt function that gets the host using the ip address of the server.
    if (server ==NULL)
    {
        fprintf(stderr, "Error! No such host.");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length); // Transfer info from *server to serv_addr. // Need to read more later.
    serv_addr.sin_port = htons(portno); // Host to network short
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        error("Connection Failed!");

    while(1) 
    {
        bzero(buffer, 255); // Clear the buffer
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            error("Error on writing from client.");
        
        bzero(buffer, 255);

        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("Error on reading from server");

        printf("Server: %s", buffer);

        int i = strncmp("Bye", buffer, 3);
        if (i==0)
            break;

    }

    return 0;

}

