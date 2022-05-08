/*

filename server_ipaddress portno

argv[0] filename
argv[1] server_ipaddress
argv[2] portno

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // use for read write and close
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> /* use for hostent*/

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) 
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    /*The hostent structure is used by functions to store
     information about a given host, such as host name,
      IPv4 address,
    */
    struct hostent *server;

    char buffer[256];

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]); 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]); 
    if (server ==NULL)
    {
        fprintf(stderr, "Error! No such host.");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // bcopy will help us to copy from *server to serv_addr
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length); 
    // takes a 16-bit number in host byte order and returns a 16-bit number in network byte order used in TCP/IP networks
    serv_addr.sin_port = htons(portno); 

    // connect
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        error("Connection Failed!");

    while(1) 
    {
        // clear
        bzero(buffer, 255); 
        // required to pass string to server
        fgets(buffer, 255, stdin);
        // write to server
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            error("Error on writing from client.");
        // clean again to get from server
        bzero(buffer, 255);
        //read
        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("Error on reading from server");

        printf("Server: %s", buffer);
        // if bye exit
        int i = strncmp("Bye", buffer, 3);
        if (i==0)
            break;

    }

    return 0;

}

