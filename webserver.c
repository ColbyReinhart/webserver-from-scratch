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

	// PREPARE SERVER SOCKET
	int listen_socket = make_server_socket(portnum);
	if (listen_socket == -1) exit(1);

	// Accept incoming call
	printf("Waiting for new calls . . .\n");
	while(1)
	{
		// Get a client connection
		int client_connection = accept(listen_socket, NULL, NULL);
		if (client_connection == -1) errorQuit("Couldn't accept a socket call");

		// Handle the call
		if (handle_call(client_connection) == -1)
		{
			close(client_connection);
			exit(1);
		}
		close(client_connection);
	}
}