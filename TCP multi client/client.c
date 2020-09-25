#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include <arpa/inet.h>

#define PORT 8000
#define SIZE 1024

int main()
{

    int sockfd;
    struct sockaddr_in serverAddress;//client will connect on this

    char message[SIZE];

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    
    //initialize the socket addresses
    memset(&serverAddress,0,sizeof(serverAddress));
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.s_addr=inet_addr("127.0.0.1");
    serverAddress.sin_port=htons(PORT);

    //client  connect to server on port
    connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));

    // connect identifies ephimeral port no which is attached to sockfd
    
    //send to sever and receive from server
    while(1)
    {
        printf("Client: ");
        fflush(stdin);
        fgets(message,SIZE,stdin);
        
        int n = strlen(message) + 1;
        send(sockfd,message,n,0);

        if(strncmp(message,"bye",3) == 0) {
            printf("Disconnecting from server\n");   
            break;
        }

        n = recv(sockfd,message,SIZE,0);

        printf("Server: %s\n",message);
    }

 return 0;
}