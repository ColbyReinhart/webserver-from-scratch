// A simple webserver from scratch
// Author: Colby Reinhart
// 5-15-2022

#include "webserver.h"

#define errorQuit(msg) { perror(msg); exit(1); }

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

	// Setup signal handlers
	signal(SIGCHLD, child_waiter);

	// Accept incoming calls
	while(1)
	{
		// Get a client connection
		int client_connection = accept(listen_socket, NULL, NULL);
		if (client_connection == -1 && errno != EINTR) errorQuit("Couldn't accept a socket call");

		// Tell a child to handle the call
		if (fork() == 0)
		{
			handle_call(client_connection);
			close(client_connection);
			exit(0);
		}
		else
		{
			close(client_connection);
		}
	}
}