#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> // to use IP
#include <arpa/inet.h> // inet_addr
#include <unistd.h> // close()
#include <string.h>

int main() {

	int mainSocket;
	int newSocket; // handles clients
	struct sockaddr_in serverAddr; 
	struct sockaddr_in clientAddr; // stores address of client who's request is accepted

	mainSocket = socket(AF_INET, SOCK_STREAM,0);

	serverAddr.sin_family = AF_INET; // Address family -> Internet
	serverAddr.sin_port = htons(7891); // assigning port to server 
	// htons: host to newtork short
	// htons converts port no to network byte ordered format
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // initialise IP address with loopback address
	// inet_addr convert dotted decimal notation IP address to network byte ordered IP address [ in_addr_t ]
	memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero)); // last 8 bytes initialised to NULL

	// bind dummmy socket to IP and port no to make it a valid socket
	int res = bind(mainSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

	if(res == -1) {
		printf("Error in binding\n");
		return 0;
	}

	// Max backlog size is 5
	// Types of backlog: (1) 3WHS left (2) 3WHS done but resource allocation at server left 
	if(listen(mainSocket, 5) == 0) {
		printf("Listening at Server\n");
	} else {
		printf("Error in Listening\n");
	}

	socklen_t addr_size = sizeof(clientAddr); 

	// accept call creates new socket for incoming connection
	// client IP + port no will be stored in clientAddr from the kernel as
	// accept system call is kernel to process
	newSocket = accept(mainSocket, (struct sockaddr*) &clientAddr, &addr_size);

	if(newSocket == -1) {
		printf("Error in accepting\n");
		return 0;
	} else {
		printf("Connection established with client\n\n");
	}

	char message[1024], exit[10];
	strcpy(exit,"exit");

	while(1) {
		printf("Server: ");
		fgets(message, 1024, stdin);
		send(newSocket, message, sizeof(message), 0);

		if(strcmp(message, exit) == 10) {
			break;
		}

		recv(newSocket, message, sizeof(message), 0);
		printf("Client: %s", message);
	}

	
	close(newSocket); 
	close(mainSocket); 
}