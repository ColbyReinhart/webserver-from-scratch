// webserver.c
// Webserver From Scratch main functionality
// Written by Colby Reinhart
// Revamped 8-25-2022

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
// #include <sys/socket.h>
// #include <sys/wait.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <string.h>
#include <signal.h>
// #include <fcntl.h>
#include <errno.h>

#include "webserver.h"
#include "utilities.h"

void handle_call(const int client_fd);

// Handle child termination
void child_waiter(const int signum)
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char* argv[])
{
	// TODO: allow output log file as parameter

	// Prepare server socket
	const int listen_socket = make_server_socket(WEB_PORT);

	// Setup child handler
	signal(SIGCHLD, child_waiter);

	// Accept incoming calls
	while (1)
	{
		// Get a client connection
		const int client = accept(listen_socket, NULL, NULL);
		if (client == -1 && errno != EINTR)
		{
			perror("Error accepting client connection");
			exit(1);
		}

		// Tell a child to handle the call
		if (fork() == 0)
		{
			handle_call(client);
			close(client);
			exit(0);
		}
		else
		{
			close(client);
		}
	}
}

// Handle a call received from a client
// Input: the file descriptor for the connection
void handle_call(const int client_fd)
{
	// Read in the request
	char request[MAX_REQ_LENGTH];
	bzero(request, MAX_REQ_LENGTH);
	if (read(client_fd, request, MAX_REQ_LENGTH) == -1)
	{
		perror("Couldn't read client request");
		exit(1);
	}

	// Split the request into the head and body
}