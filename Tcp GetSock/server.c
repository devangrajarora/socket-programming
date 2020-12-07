#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> // to use IP
#include <arpa/inet.h> // inet_addr
#include <unistd.h> // close()
#include <string.h>
#define SIZE 1024
#define PORT 7891

int main(){
  int welcomeSocket, newSocket;
  socklen_t len;
  char buffer[SIZE];
  struct sockaddr_in serverAddr, sock_addr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
	
  printf("ACCEPTED...\n");
  /*---- Send message to the socket of the incoming connection ----*/
  strcpy(buffer,"Hello World\n");

  send(newSocket,buffer,sizeof(buffer),0);

  len = sizeof(sock_addr);
	
  if (getsockname(newSocket, (struct sockaddr *)&sock_addr, &len ) == -1){
    	perror("getsockname");
    	close(newSocket);
    	return(-1);
  }
	
	
  printf("sockname: sock_addr.sin_port=%d \n",ntohs(sock_addr.sin_port)); 
  printf("sockname: sock_addr.sin_family=%d \n",sock_addr.sin_family);
  printf("sockname: sock_addr.sin_addr=%s \n",inet_ntoa(sock_addr.sin_addr));
		
  if (getpeername(newSocket, (struct sockaddr *)&sock_addr, &len ) == -1) {
        perror("getpeername");
        close(newSocket);
        return(-1);
   }
	
	
  printf("peername: sock_addr.sin_port=%d \n",ntohs(sock_addr.sin_port)); 
  printf("peername: sock_addr.sin_family=%d \n",sock_addr.sin_family);
  printf("peername: sock_addr.sin_addr=%s \n",inet_ntoa(sock_addr.sin_addr));
	
  close(newSocket);
  close(welcomeSocket);
  
  return 0;
}
