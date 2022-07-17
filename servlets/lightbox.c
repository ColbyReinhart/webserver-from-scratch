// lightbox.c
// Servlet for handling lightbox communication
// Author: Colby Reinhart
// 5-28-2022

#include "../webserver.h"

int lightbox_socket = -1;

int serve_lightbox(int sock_fd, char* requested_file)
{
	if (strcmp(requested_file, "connect.action") == 0)
	{
		lightbox_socket = sock_fd;
		const char* response = "r=255 g=0 b=0";
		write(sock_fd, response, strlen(response));
	}
	else
	{
		// Worry about this later
		send_empty_response(sock_fd, http_501);
	}
}