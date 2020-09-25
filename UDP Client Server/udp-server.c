#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SIZE 1024
#define PORT 8080

int main() {

	int sockfd;
	char buffer[SIZE];
	char message[SIZE];
	strcpy(message,"Hello from Server");

	struct sockaddr_in serverAddr, clientAddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd == -1) {
		printf("Socket not created\n");
		return 0;
	} else {
		printf("Socket created\n");
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	memset(&clientAddr, 0, sizeof(clientAddr));

	// filling server info
	serverAddr.sin_family = AF_INET; // IPv4
	serverAddr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 
	serverAddr.sin_port = htons(PORT); // convert port to network byte order

	// bind socket with server address
	int ret = bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)); // process to kernel

	// bind() of INADDR_ANY does NOT "generate a random IP". 
	// It binds the socket to all available interfaces.
	// For a server, you typically want to bind to all interfaces - not just "localhost"
	// using this socket can accept connections from all interfaces in the machine

	// https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming#:~:text=INADDR_ANY%20is%20used%20when%20you,the%20IPs%20of%20the%20machine.

	if(ret == -1) {
		printf("Bind failed\n");
		return 0;
	} else {
		printf("Bind successful\n");
	}

	socklen_t len = sizeof(clientAddr);

	// Here, recv from is working as accept()	

	// kernel to process
	// return bytes of data read
	// client data is written to clientAddr and then is available to the program as recvfrom is kernel to process 
	int n = recvfrom(sockfd, buffer, SIZE, MSG_WAITALL, (struct sockaddr*) &clientAddr, &len);

	// MSG_WAITALL -> wait until all data is read 

	buffer[n] = '\0';
	printf("Client said: %s\n", buffer);

	// process to kernel
	sendto(sockfd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&clientAddr, len);

	close(sockfd);
	return 0;
}

