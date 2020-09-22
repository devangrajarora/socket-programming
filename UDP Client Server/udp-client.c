#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080
#define SIZE 1024

int main() {

	char buffer[SIZE], message[SIZE];
	strcpy(message,"Hi server");

	int sockfd;
	struct sockaddr_in serverAddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd == -1) {
		printf("Socket not created\n");
		return 0;
	} else {
		printf("Socket created\n");
	}

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY; // this will be overwritten by server IP at recvfrom
	// INADDR_ANY is broadcast address, message will go to all listeners on the network


	socklen_t len = sizeof(serverAddr);

	// sendto works like connect()
	sendto(sockfd, message, strlen(message), MSG_CONFIRM, (struct sockaddr *)&serverAddr, len);

	int n = recvfrom(sockfd, buffer, SIZE, MSG_WAITALL, (struct sockaddr *) &serverAddr, &len);
	buffer[n] = '\0';

	printf("Server said: %s\n", buffer);

	close(sockfd);
	return 0;
}