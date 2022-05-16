// Code for handling incoming calls
// Author: Colby Reinhart
// 5-15-2022

#include "webserver.h"

char request[MAX_REQ_LENGTH];

// A structure to define a list of route entries in tabular form
struct routeEntry
{
	char* path;
	char* route;
};

struct routeEntry routeTable[] =
{
	"/"		,	"index.html",
	NULL	,	NULL
};

int handle_call(int sock_fd)
{
	// Empty the request buffer
	bzero(request, MAX_REQ_LENGTH);

	// Read in the request
	int bytes_read = read(sock_fd, request, MAX_REQ_LENGTH);
	if (bytes_read == -1)
	{
		perror("Couldn't read client request");
		return -1;
	}
	char* requestType = strtok(request, " \r\n");
	char* requestPath = strtok(NULL, " \r\n");

	printf("%s request at %s\n", requestType, requestPath);///

	// What kind of request is this?
	if (strcmp(requestType, "GET") != 0)
	{
		printf("Error: unsupported request type: %s\n", requestType);
		return -1;
	}

	// Check routing table
	int validRoute = 0;
	char* responseRoute;
	for (int i = 0; routeTable[i].path != NULL; ++i)
	{
		if (strcmp(requestPath, routeTable[i].path) == 0)
		{
			strcpy(responseRoute, routeTable[i].route);
			validRoute = 1;
		}
	}
	
	if (validRoute)
	{
		// Send back success header
		if (write(sock_fd, HTTP_200, strlen(HTTP_200)) == -1)
		{
			perror("Couldn't respond to client");
			return -1;
		}

		// Send over html
		char* filePath;
		strcpy(filePath, SITES_PATH);
		strcat(filePath, responseRoute);
		FILE* webpageToServe = fopen(filePath, "r");
		char buffer[BUFSIZ];
		bytes_read = 0;
		
		do
		{
			bytes_read = fread(buffer, 1, BUFSIZ, webpageToServe);
			write(sock_fd, buffer, bytes_read);
		} while(bytes_read > 0);
	}
	else
	{
		// Send back 404 error
		if (write(sock_fd, HTTP_404, strlen(HTTP_404)) == -1)
		{
			perror("Couldn't respond to client");
			return -1;
		}
	}

	return 0;
}