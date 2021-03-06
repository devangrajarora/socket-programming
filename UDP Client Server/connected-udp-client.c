#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include<unistd.h> 
#include<stdlib.h> 

#define PORT 8080 
#define MAXLINE 1000 

// Connected UDP is used to make UDP reliable
// reduces number of communications on the network -> reduces overhead

// In connected UDP, port+IP of peer is recorded

// connection oriented like TCP but less overhead as stuff like retransmission, sequence numbering etc is not present	
// still independent datagrams are sent but now it's a connection is formed first, so communication is reliable

// Driver code 
int main() 
{ 
	char buffer[100]; 
	char *message = "Hello Server"; 
	int sockfd, n; 
	struct sockaddr_in servaddr; 
	
	// clear servaddr 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_family = AF_INET; 
	
	// create datagram socket 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	
	// connect to server 
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
	{ 
		printf("\n Error : Connect Failed \n"); 
		exit(0); 
	} 

	// virtual connection created

	// request to send datagram 
	// no need to specify server address in sendto
	// connect stores the peers IP and port -> so we send NULL
	sendto(sockfd, message, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr)); 
	
	// waiting for response 
	recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL); 
	puts(buffer); 

	// close the descriptor 
	close(sockfd); 
} 
