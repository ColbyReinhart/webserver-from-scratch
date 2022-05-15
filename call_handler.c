// Code for handling incoming calls
// Author: Colby Reinhart
// 5-15-2022

#include "webserver.h"

char request[MAX_REQ_LENGTH];

int handle_call(int sock_fd)
{
	printf("Got a call!\n");

	// Empty the buffer, just in case
	bzero(request, MAX_REQ_LENGTH);

	// Read in the request
	int bytes_read = read(sock_fd, request, MAX_REQ_LENGTH);
	if (bytes_read == -1)
	{
		perror("Couldn't read client request");
		return -1;
	}

	// What kind of request is this?
	char* token = strtok(request, " ");
	if (strcmp(token, "GET") != 0)
	{
		printf("Error: unsupported request type: %s\n", token);
		return -1;
	}

	// Send back success header
	if (write(sock_fd, HTTP_200, strlen(HTTP_200)) == -1)
	{
		perror("Couldn't respond to client");
		return -1;
	}

	// Send over html
	FILE* html = fopen("sites/hello.html", "r");
	char buffer[BUFSIZ];
	bytes_read = 0;
	do
	{
		bytes_read = fread(buffer, 1, BUFSIZ, html);
		write(sock_fd, buffer, bytes_read);
	} while(bytes_read > 0);

	return 0;
}