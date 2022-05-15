// A simple webserver from scratch
// Author: Colby Reinhart
// 5-15-2022

#include "webserver.h"

#define errorQuit(msg) { perror(msg); exit(1); }

int main(int argc, char* argv[])
{
	int portnum;

	// If the arguments are invalid
	if (argc > 2)
	{
		printf("Usage: ./server -[OPTIONAL_MODE]\nt = test mode\n");
		exit(0);
	}

	// If there's a mode flag
	else if (argc == 2)
	{
		if (strcmp(argv[1], "-t") == 0) portnum = TEST_PORT;
	}

	// If there are no mode flags
	else
	{
		portnum = WEB_PORT;
	}

	printf("Listening on port %d.\n", portnum);

	// Prepare socket stuff
	struct sockaddr_in socket_addr;						// Socket address information
	struct hostent* host_info;							// Host information

	// Ask the kernel for a socket
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == -1) errorQuit("Couldn't get socket");

	// Get host information
	bzero((void*)&socket_addr, sizeof(socket_addr));	// Clear out struct
	host_info = gethostbyname(HOSTNAME);				// Get info about host
	
	// Fill in host information
	bcopy(												// Fill host address
		(void*)host_info->h_addr,
		(void*)&socket_addr.sin_addr,
		host_info->h_length
	);
	socket_addr.sin_port = htons(portnum);				// Fill in port
	socket_addr.sin_family = AF_INET;					// Use IPV4

	// Bind address to socket
	if (bind(listen_socket, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) != 0)
	{
		errorQuit("Could not bind socket");
	}

	// Allow incoming calls
	if (listen(listen_socket, QSIZE) == -1) errorQuit("Could not listen for calls");

	// Accept incoming call
	printf("Waiting for new calls . . .\n");
	while(1)
	{
		int comm_socket = accept(listen_socket, NULL, NULL);
		if (comm_socket == -1) errorQuit("Couldn't accept a socket call");
		if (handle_call(comm_socket) == -1)
		{
			close(comm_socket);
			exit(1);
		}
		close(comm_socket);
	}
}