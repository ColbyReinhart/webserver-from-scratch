// Code for handling incoming calls
// Author: Colby Reinhart
// 5-15-2022

#include "webserver.h"

// Response headers
const struct response_entry http_200 = {200, "OK"};
const struct response_entry http_400 = {400, "Bad Request"};
const struct response_entry http_404 = {404, "Not Found"};
const struct response_entry http_501 = {501, "Not Implemented"};

// Handle a call from a client
// Input: the file descriptor of the client socket, the request string
// Returns 0 on success, -1 on failure
int handle_call(int sock_fd, char* request)
{
	// What type of request is this?
	char* request_type = strtok(request, " \r\n");
	printf("%s ", request_type);///

	// Did we get a valid token?
	if (request_type == NULL)
	{
		return send_empty_response(sock_fd, http_400);
	}

	// Is this request type supported?
	if (!is_supported_request_type(request_type))
	{
		return send_empty_response(sock_fd, http_501);
	}

	// Get the requested route
	char* request_path = strtok(NULL, " \r\n");
	printf("%s\n", request_path);///

	// Did we get a valid token?
	if (request_path == NULL)
	{
		return send_empty_response(sock_fd, http_400);
	}
	// Consult the routing table and call the appropriate servlet
	else
	{
		return call_servlet(sock_fd, request_path);
	}
}