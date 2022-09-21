#define _GNU_SOURCE
/*
	C ECHO client example using sockets
*/

#include <fcntl.h>
#include <stdio.h>		//printf
#include <string.h>		//strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#define SM 1024

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[SM], server_reply[SM];

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(3512);

	// Connect to remote server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

	puts("Connected\n");

	// keep communicating with server

	char data[SM];
  
	   while(strncmp(message, "EXIT",4)){
        printf("Enter message : ");
		 
	  scanf("%[^\n]s", message);
	  fflush(stdout);
	   while ((getchar()) != '\n');
        
		if (send(sock, message, strlen(message), 0) < 0)
		{
			
			puts("Send failed");
			return 1;
		}
		// Receive a reply from the server
		//printf("OUTPUT: %s\n", message);
		char data[SM];
		if (!strncmp(message, "POP", 3) || !strncmp(message, "TOP", 3))
		{

			 //printf("vay ");
			int nb;
			nb = recv(sock, data, 1024, 0);
			data[nb] = '\0';
            // bzero(message, 1024);
			printf("OUTPUT: %s\n", data);
		   
		}
	   // bzero(message, 1024);
	   }
	// puts("client ");

	close(sock);
	return 0;
}

