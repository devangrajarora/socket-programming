#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define PORT 8000
#define SIZE 1024

int main() {

    int sockfd; // to create new socket
    int newsockfd; // to accept connections
    char message[SIZE];

    struct sockaddr_in serverAddr; 
    struct sockaddr_in clientAddr;

    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create dummy socket

    // initialise serverAddr
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0

    // When INADDR_ANY is specified in the bind call, the socket will be bound to all local interfaces.
    // https://man7.org/linux/man-pages/man7/ip.7.html


    // bind socket with server address ans port
    int ret = bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

    if(ret == -1) {
        printf("Bind failed\n");
        return 0;
    } else {
        printf("Bind successful\n");
    }

    // listen for new connections from clients
    listen(sockfd, 5);

    while(1) { // to accept multiple clients

        socklen_t len = sizeof(clientAddr);
        newsockfd = accept(sockfd, (struct sockaddr*) &clientAddr, &len);

        if(newsockfd == -1) {
            printf("Connection establishment failed");
            continue;
        } 

        char *cliAddress = inet_ntoa(clientAddr.sin_addr);
        int cliPort = ntohs(clientAddr.sin_port);

        printf("Connected to Client: %s at port %d\n", cliAddress, cliPort);

        pid = fork(); // child created to service new client

        if(pid == 0) { // child process is responsible for sending and recieving messages

            while(1) {
                int n = recv(newsockfd, message, SIZE, 0);

                message[n] = '\0';
                printf("\nClient %d: %s",cliPort,message);

                // printf("%d",strcmp(message,"bye"));
                if(strncmp(message,"bye",3) == 0) {
                    printf("Disconnecting from client %d", cliPort);  
                    close(newsockfd);
                    break;
                }
                
                fflush(stdin);
                printf("Server: ");
                fgets(message,SIZE,stdin);
                send(newsockfd, message, SIZE, 0);
            }

            close(newsockfd);
            exit(0);
        } else {
            
            close(newsockfd); // parent closes newsockfd and listens for incomig client requests
        }
    }

    close(sockfd);
    return 0;
}