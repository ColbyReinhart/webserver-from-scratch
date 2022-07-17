// A simple webserver from scratch
// Author: Colby Reinhart
// 5-15-2022

#include "webserver.h"

#define errorQuit(msg) { perror(msg); exit(1); }

int lightbox_socket;

// Handles child termination
void child_waiter(int signum)
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char* argv[])
{
	printf("%s\n", ROOT_PATH);
	
	// Prepare server socket
	int listen_socket = make_server_socket(WEB_PORT);
	if (listen_socket == -1) exit(1);
	lightbox_socket = -1;

	// Setup signal handlers
	signal(SIGCHLD, child_waiter);

	// Accept incoming calls
	while(1)
	{
		// Get a client connection
		int client_connection = accept(listen_socket, NULL, NULL);
		if (client_connection == -1 && errno != EINTR) errorQuit("Couldn't accept a socket call");

		// Read in the request
		char request[MAX_REQ_LENGTH];
		bzero(request, MAX_REQ_LENGTH);
		int bytes_read = read(client_connection, request, MAX_REQ_LENGTH);
		if (bytes_read == -1)
		{
			errorQuit("Couldn't read client request");
		}

		// What type of request is this?
		char* request_type = strtok(request, " ");

		// If this is a CONNECT call, hold onto it
		const int isConnectCall = strcmp(request_type, "CONNECT") == 0;

		// Tell a child to handle the call
		if (fork() == 0)
		{
			handle_call(client_connection, request);
			close(client_connection);
			exit(0);
		}
		else
		{
			if (isConnectCall)
			{
				close(lightbox_socket);
				lightbox_socket = client_connection;
			}
			else
			{
				close(client_connection);
			}
		}
	}
}