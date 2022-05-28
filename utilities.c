// Webserver utility functions
// Author: Colby Reinhart
// 5-15-2022

#include "webserver.h"

//
// Declarations
//

// Supported request types
const char* supported_request_types[] = {"GET", NULL};

// Content Types
struct content_type_entry
{
	char* extension;
	char* type;
};
struct content_type_entry content_types[] =
{
	"html"	, 	"text/html",
	"css"	,	"text/css",
	"js"	,	"text/javascript",
	NULL	,	NULL
};

// Route table entries in tabular form
struct route_entry
{
	char* path;
	char* route;
};
struct route_entry route_table[] =
{
	"/"						,	"/homepage/",
	NULL					,	NULL
};

// Prototypes
void get_content_type(char* file_path, char* content_type);

//
// Definitions
//

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

// Check whether to request type is supported or not
// Input: the request to validate
// Output: 1 on supported, 0 if not
int is_supported_request_type(char* request_type)
{
	for (int i = 0; supported_request_types[i] != NULL; ++i)
	{
		if (strcmp(supported_request_types[i], request_type) == 0) return 1;
	}

	return 0;
}

// Send a response with no body (typically 400-500 codes)
// Input: the file descriptor to write to and the response type
// Returns 0 on success, -1 on error
int send_empty_response(int sock_fd, struct response_entry response)
{
	char response_text[MAX_REQ_LENGTH]; // Arbitrarily large
	sprintf
	(
		response_text,
		"HTTP/1.1 %d %s\r\n\r\n",
		response.code,
		response.message
	);
	if (write(sock_fd, response_text, strlen(response_text)) == -1)
	{
		perror("Couldn't respond to client");
		return -1;
	}

	return 0;
}

// Send a response with a body and the appropriate headers
int send_normal_response(int sock_fd, struct response_entry response, char* file_path)
{
	// Get the content-type header info
	char content_type[50];
	get_content_type(file_path, content_type);

	// Send the header
	char response_text[MAX_REQ_LENGTH]; // Arbitrarily large
	sprintf
	(
		response_text,
		"HTTP/1.1 %d %s\r\nContent-type: %s; charset=utf-8\r\n\r\n",
		response.code,
		response.message,
		content_type
	);
	if (write(sock_fd, response_text, strlen(response_text)) == -1)
	{
		perror("Couldn't respond to client");
		return -1;
	}

	// Read the requested file and write it to the socket
	FILE* webpage_to_serve = fopen(file_path, "r");
	FILE* socket = fdopen(sock_fd, "w");
	char buffer[BUFSIZ];
	int bytes_read = 0;
	do
	{
		bytes_read = fread(buffer, 1, BUFSIZ, webpage_to_serve);
		fwrite(buffer, 1, bytes_read, socket);
	} while (bytes_read > 0);
	fclose(webpage_to_serve);
	fclose(socket);

	return 0;
}

// Get the content type of the requested file
// Input: the requested file (filepath may be included) and a char* to be
// filled with the content type (defaults to "text/plain")
void get_content_type(char* file_path, char* content_type)
{
	char* extension = strrchr(file_path, '.') + 1;
	for (int i = 0; content_types[i].extension != NULL; ++i)
	{
		if (strcmp(extension, content_types[i].extension) == 0)
		{
			strcpy(content_type, content_types[i].type);
			return;
		}
	}

	strcpy(content_type, "text/plain");
}

// Consult the routing table to serve the appropriate webpage
// Input: the requested route and a char* to fill with the result
// (empty string if no route found)
// Returns 1 if a route is found, 0 if not
int get_server_route(char* url_route, char* route_to_fill)
{
	// If the last character isn't a slash and we're not grabbing a file, add a slash
	if (url_route[strlen(url_route) - 1] != '/' && strrchr(url_route, '.') == NULL)
	{
		strcat(url_route, "/");
	}

	// Get the file and folder
	const int array_size = 100; // Arbitrary
	int last_slash_pos = strrchr(url_route, '/') - url_route;

	// Folder
	char requested_folder[array_size];
	bzero(requested_folder, array_size);
	strncpy(requested_folder, url_route, last_slash_pos + 1);
	requested_folder[last_slash_pos + 1] = 0; // Add a null terminator

	// File
	char requested_file[array_size];
	bzero(requested_file, array_size);
	strcpy(requested_file, url_route + last_slash_pos + 1);

	// If requested_file is empty, fill it with index.html
	if (strlen(requested_file) == 0) strcpy(requested_file, "index.html");

	// Consult the routing table to look for the folder
	int valid_route = 0;
	for (int i = 0; route_table[i].path != NULL; ++i)
	{
		if (strcmp(requested_folder, route_table[i].path) == 0)
		{
			valid_route = 1;
			strcpy(requested_folder, route_table[i].route);
		}
	}

	// Return the final result
	if (valid_route)
	{
		strcpy(route_to_fill, ROOT_PATH);			// Root path
		strcat(route_to_fill, requested_folder);	// Folder
		strcat(route_to_fill, requested_file);		// File
		return 1;
	}
	
	return 0;
}