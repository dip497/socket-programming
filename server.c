// Server port number must be greater than 1024 as ports 1-1024 are reserved ports for the system.
// 65536 ports in total.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) 
{
    perror(msg); // Interprets error number and outputs error desc using stderr
    exit(1); // We require our program to terminate when we write the error function
}

int main(int argc, char *argv[]) 
// These argc and argv[] refer to the command line arguments giver when the file is run in the terminal.
// argc - total no of parameters including the filename (here, 2) 
// argv[] is the array formed from the command line arguments. argv[0] = filename 
// Refer : https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
{
    if (argc < 2)
    {
        fprintf(stderr, "Port number not provided. Program terminated.\n");
        exit(1); // Represents EXIT_FAILURE or abnormal exit while exit(0) represents EXIT_SUCCESS or successful termination.
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr;  // sockaddr_in gives us internet address <netinet.h>
    socklen_t clilen; //socklen is a data type in <socket.h> 32 bit

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) // i.e., socket() function resulted in failure.
        error("Error opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr)); // bzero() clears any data or text given inside as parameters.
    
    portno = atoi(argv[1]); // argv[1] contains the port no specified with command line arguments.

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno); // htons() - host to network short also, see host to network long

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Binding Failed"); // If function fails, bind will return -1.

    listen(sockfd, 5); // 5 is the max no of clients that can connect to the server.
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0) // accept() fails
        error("Error on accept");

    while(1) // Code for communication between client and server.
    {
        bzero(buffer, 255); //  Clear the buffer. We have defined the buffer size as 255.
        n = read(newsockfd, buffer, 255);
        if (n < 0) // read() fails
            error("Error on reading.");
        printf("Client: %s\n", buffer);

        bzero(buffer, 255); // Clear the buffer so that we can use it to transmit the server's message.
        fgets(buffer, 255, stdin); // <stdio.h> function reads bytes from stream.
        n = write(newsockfd, buffer, strlen(buffer));
        // fgets() reads from server, and write function sends it over the network.
        if (n < 0) {
            error("Error on writing");
        }
        int i = strncmp("Bye", buffer, 3); // Inbuilt function for string comparison
        if (i==0)
            break;
    }
    close(newsockfd);
    close(sockfd);

    return 0;

}