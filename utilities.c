// Webserver utility functions
// Author: Colby Reinhart
// 5-15-2022

#include "webserver.h"

// Setup log files
// Returns 0 on success, -1 on error
int log_setup()
{
	// First set up services.log
	char file_path[100];
	strcpy(file_path, LOGFILE_PATH);
	strcat(file_path, "services.log");
	int fd = open(file_path, O_WRONLY);
	if (fd != 3)
	{
		printf("Couldn't open log file in fd = 3.\n");
		printf("Make sure you call logSetup() before opening anything else!");
		return -1;
	}

	// Now set up errors.log (redirect stderr)
	bzero(file_path, 100);
	strcpy(file_path, LOGFILE_PATH);
	strcat(file_path, "errors.log");
	fd = open(file_path, O_WRONLY);
	int new_fd = dup2(fd, 2);
	if (new_fd != 2)
	{
		perror("Couldn't open error log file");
		return -1;
	}

	return 0;
}

// Attempt a graceful exit
// Input: the number to provide to exit()
void cleanup_and_exit(int exit_number)
{
	close(ERROR_FD);
	close(LOG_FD);
	exit(exit_number);
}

// Make a server socket and return it
// Input: the port number to tell the server to listen on
// Output: the file descriptor of the created socket (-1 if error)
int make_server_socket(int port)
{
	struct sockaddr_in socket_addr;						// Socket address information
	struct hostent* host_info;							// Host information

	// Ask the kernel for a socket
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == -1)
	{
		perror("Couldn't get socket");
		return -1;
	}

	// Get host information
	bzero((void*)&socket_addr, sizeof(socket_addr));	// Clear out struct
	host_info = gethostbyname(HOSTNAME);				// Get info about host

	// Fill in host information
	bcopy(												// Fill host address
		(void*)host_info->h_addr,
		(void*)&socket_addr.sin_addr,
		host_info->h_length
	);
	socket_addr.sin_port = htons(port);					// Fill in port
	socket_addr.sin_family = AF_INET;					// Use IPV4

	// Allow the socket to be immediately reused after closing
	const int opt = 1;
	setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// Bind address to socket
	if (bind(listen_socket, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) == -1)
	{
		perror("Could not bind to socket");
		return -1;
	}

	// Allow incoming calls
	if (listen(listen_socket, QSIZE) == -1)
	{
		perror("Could not listen for calls");
		return -1;
	}

	return listen_socket;
}