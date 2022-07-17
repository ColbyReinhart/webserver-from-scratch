// lightbox.c
// Servlet for handling lightbox communication
// Author: Colby Reinhart
// 5-28-2022

#include "../webserver.h"

extern int lightbox_socket;

int serve_lightbox(int sock_fd, char* requested_file)
{
	if (strcmp(requested_file, "connect.action") == 0)
	{
		return 100;
	}
	else if (strcmp(requested_file, "red.action") == 0)
	{
		const char* response = "255 0 0\r\n";
		write(lightbox_socket, response, strlen(response));
		return send_empty_response(sock_fd, http_200);
	}
	else if (strcmp(requested_file, "green.action") == 0)
	{
		const char* response = "0 255 0\r\n";
		write(lightbox_socket, response, strlen(response));
		return send_empty_response(sock_fd, http_200);
	}
	else if (strcmp(requested_file, "blue.action") == 0)
	{
		const char* response = "0 0 255\r\n";
		write(lightbox_socket, response, strlen(response));
		return send_empty_response(sock_fd, http_200);
	}
	else
	{
		// Worry about this later
		send_empty_response(sock_fd, http_501);
	}
}