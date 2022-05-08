

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) 
{
    perror(msg); 
    exit(1); 
}
// argv[0] is the file name and argv[1] is portno

int main(int argc, char *argv[]) 
{
    if (argc < 2)
    {
        fprintf(stderr, "Port number not provided. Program terminated.\n");
        exit(1); 
    }
    // SOCKFD IS FILE descriptor
    // NEWSOCKFD IS NEW FILE descriptor, required after connection
    // portno is the port number
    // n is function sucessfull or not
    // buffer[can be any number] first store in buffer and then pass in datastream
    

    int sockfd, newsockfd, portno, n;
    char buffer[255];

    //  {sockaddr_in gives internet address}
    // chilen is length of sock  {socklen_t is datatype of 32 bit }

    struct sockaddr_in serv_addr, cli_addr;  
    socklen_t clilen; 

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //AF_INET IS FOR IPV4 sockets
    // SOCK_STREAM IS FOR TCP 
    // 0 IS FOR TCP 


    
    if (sockfd < 0) 
        error("Error opening socket");

    
    
    //bzero erases the data 
    bzero((char *) &serv_addr, sizeof(serv_addr)); 
    
    //atoi convert string into integer

    portno = atoi(argv[1]); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno); 

    //** bind
    // sockfd is type caster to sockaddr

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Binding Failed");

    // ** listen
    // 5 stands for max number of connections can be any number
    listen(sockfd, 5); 
    clilen = sizeof(cli_addr);


    // ** accept connection


    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0) 
        error("Error on accept");

    while(1) 
    {   // clear buffer
        bzero(buffer, 255);
        //  read
        n = read(newsockfd, buffer, 255);
        if (n < 0) 
            error("Error on reading.");
        printf("Client: %s\n", buffer);
        // clear buffer
        bzero(buffer, 255); 
        // fget will get replay from server
        fgets(buffer, 255, stdin); 
        n = write(newsockfd, buffer, strlen(buffer));
       
        if (n < 0) {
            error("Error on writing");
        }
        // strncmp inbuild function to compare string
        int i = strncmp("Bye", buffer, 3); 
        if (i==0)
            break;
    }
    close(newsockfd);
    close(sockfd);

    return 0;

}