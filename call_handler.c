// Code for handling incoming calls
// Author: Colby Reinhart
// 5-15-2022

#include "webserver.h"

// Response headers
struct responseEntry
{
	int code;
	char* message;
};
const struct responseEntry http_200 = {200, "OK"};
const struct responseEntry http_404 = {404, "Not Found"};
const struct responseEntry http_501 = {501, "Not Implemented"};

// Content Types
struct contentTypeEntry
{
	char* extension;
	char* type;
};
struct contentTypeEntry contentTypes[] =
{
	"html"	, 	"text/html",
	"css"	,	"text/css",
	"js"	,	"text/javascript",
	NULL	,	NULL
};

// Route table entries in tabular form
struct routeEntry
{
	char* path;
	char* route;
};
struct routeEntry routeTable[] =
{
	"/"						,	"/personal-website/",
	"/webapps/roku-remote/"	,	"/webapps/roku-remote/",
	NULL					,	NULL
};

// Other declarations
char request[MAX_REQ_LENGTH];
int sendServerError(int sock_fd);
int sendResponse(int sock_fd, struct responseEntry response, char* requestedFile);
int getServerRoute(char* urlRoute, char* routeToFill);

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

	// What kind of request is this?
	if (strcmp(requestType, "GET") != 0)
	{
		printf("Error: unsupported request type: %s\n", requestType);
		sendResponse(sock_fd, http_501, NULL);
		return 0;
	}

	// Check routing table
	char responseRoute[BUFSIZ];
	int validRoute = getServerRoute(requestPath, responseRoute);
	
	if (validRoute)
	{
		if (sendResponse(sock_fd, http_200, responseRoute) == -1) return -1;
	}
	else
	{
		if (sendResponse(sock_fd, http_404, NULL) == -1) return -1;
	}

	return 0;
}

// Convert a URL route into a server route
// Input: the URL route to convert, the server route to fill (NULL if none)
// Output: 1 if the url route is valid, 0 if not
int getServerRoute(char* urlRoute, char* routeToFill)
{
	// First parse the url route to get the path without the last file
	int lastSlashPos = strrchr(urlRoute, '/') - urlRoute;
	char folder[BUFSIZ];
	bzero(folder, BUFSIZ);
	strncpy(folder, urlRoute, lastSlashPos + 1);

	// Now see if that folder is a valid route
	for (int i = 0; routeTable[i].path; ++i)
	{
		if (strcmp(folder, routeTable[i].path) == 0)
		{
			// Build the final path
			strcpy(routeToFill, ROOT_PATH);
			strcat(routeToFill, routeTable[i].route);
			strcat(routeToFill, urlRoute + lastSlashPos + 1);
			
			// If there is no final file, use index.html
			if (routeToFill[strlen(routeToFill) - 1] == '/')
			{
				strcat(routeToFill, "index.html");
			}
			printf("%s\n", routeToFill);///

			// Return success
			return 1;
		}
	}

	return 0;
}

// Send a 500 response
// Input: the file descriptor of the socket to send to
// Output: 0 on success, -1 on error
int sendServerError(int sock_fd)
{
	if (write(sock_fd, "HTTP/1.1 500 Internal Server Error", BUFSIZ) == -1)
	{
		perror("Couldn't respond to client");
		return -1;
	}
	return 0;
}

// Build and send a response
// Input: the file descriptor to send the response to, the response,
// and the filepath of the file to send (NULL if no file to send)
// Output: 0 on success, -1 on error
int sendResponse(int sock_fd, struct responseEntry response, char* requestedFile)
{
	char responseText[BUFSIZ];
	
	// If the filepath is NULL, then this is an error response
	if (requestedFile == NULL)
	{
		// Format response
		sprintf
		(
			responseText,
			"HTTP/1.1 %d %s\r\n\r\n",
			response.code,
			response.message
		);

		// Send response
		if (write(sock_fd, responseText, strlen(responseText)) == -1)
		{
			perror("Couldn't respond to client");
			return -1;
		}
	}

	// If the filepath is something valid, then we'll send that file
	else
	{
		// First we need to get the file extension to send the content type
		char* fileExtension = strrchr(requestedFile, '.');
		++fileExtension;

		// Now set the appropriate content type
		char* contentType = "text/plain";
		for (int i = 0;contentTypes[i].extension; ++i)
		{
			if (strcmp(contentTypes[i].extension, fileExtension) == 0)
			{
				contentType = contentTypes[i].type;
				break;
			}
		}

		// Format response header
		sprintf
		(
			responseText,
			"HTTP/1.1 %d %s\r\nContent-type: %s; charset=utf-8\r\n\r\n",
			response.code,
			response.message,
			contentType
		);

		// Send response header
		if (write(sock_fd, responseText, strlen(responseText)) == -1)
		{
			perror("Couldn't respond to client");
			return -1;
		}

		// Read the requested file and write it to the socket
		FILE* webpageToServe = fopen(requestedFile, "r");
		char buffer[BUFSIZ];
		int bytes_read = 0;
		do
		{
			bytes_read = fread(buffer, 1, BUFSIZ, webpageToServe);
			write(sock_fd, buffer, bytes_read);
		} while(bytes_read > 0);
	}

	return 0;
}