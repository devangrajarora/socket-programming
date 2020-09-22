#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

int main() {

	int clientSocket;
	struct sockaddr_in serverAddr;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0); // create dummy socket

	// initialize withs server info
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	socklen_t addr_size = sizeof(serverAddr);

	// connect socket to server
	// connect system call is process to kernel
	int ret = connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
	// clientSocket will now be attached with IP+port of the client, and this info will be stored in newSocket in server suring accept()

	if(ret == -1) {
		printf("Couldn't connect from client\n");
		return 0;
	}

	// connection bw server and client is established

	char buffer[1024], exit[10];
	strcpy(exit,"exit");

	while(true) {
		recv(clientSocket, buffer, 1024, 0);
		printf("Server: %s",buffer);
		if(strcmp(buffer, exit) == 10) {
			break;
		}

		printf("Client: ");
		fgets(buffer, 1024, stdin);
		send(clientSocket, buffer, sizeof(buffer), 0);
	}

	strcpy(buffer, "cool bye");
	send(clientSocket, buffer, sizeof(buffer), 0);

	close(clientSocket);
}