// utilities.c
// Utility functions
// Written by Colby Reinhart
// Revamped 8-25-2022

#include <sys/socket.h>
#include <netinet/in.h>

#include "utilities.h"

// Make a server socket and return it
// Output: the file descriptor of the created socket
// This function terminates program execution on failure
int make_server_socket()
{
	struct sockaddr_in socket_addr;	// Socket address information

	// Ask the kernel for a socket
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == -1)
	{
		perror("Couldn't get listening socket");
		exit(1);
	}

	// Fill in host information
	socket_addr.sin_port = htons(WEB_PORT);		// Fill in port
	socket_addr.sin_family = AF_INET;			// Use IPV4
	socket_addr.sin_addr.s_addr = INADDR_ANY;	// Use any available address

	// Allow the socket to be immediately reused after closing
	const int opt = 1;
	setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// Bind address to socket
	if (bind(listen_socket, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) == -1)
	{
		perror("Could not bind to socket");
		exit(1);
	}

	// Allow incoming calls
	if (listen(listen_socket, QSIZE) == -1)
	{
		perror("Could not listen for calls");
		exit(1);
	}

	return listen_socket;
}

// Read and format a request into a client_req structure for easy access
// Input: file descriptor to read from, client_req struct to fill
// This function will terminate execution if it fails
void read_req(const char* req, struct client_req structure)
{
	
}