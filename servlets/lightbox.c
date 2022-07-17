// lightbox.c
// Servlet for handling lightbox communication
// Author: Colby Reinhart
// 5-28-2022

#include "../webserver.h"

extern int lightbox_socket;

int serve_lightbox(int sock_fd, char* requested_file)
{
	printf("Lightbox servlet called\n");///
	if (strcmp(requested_file, "connect.action") == 0)
	{
		const char* response = "r=255 g=255 b=255\r\n";
		const int code = write(lightbox_socket, response, strlen(response));
		printf("Code: %i\n", code);
		if (code == -1)
		{
			perror("What happened: ");
		}
		return 100;
	}
	else if (strcmp(requested_file, "red.action") == 0)
	{
		const char* response = "r=255 g=0 b=0\r\n";
		write(lightbox_socket, response, strlen(response));
		return send_empty_response(sock_fd, http_200);
	}
	else if (strcmp(requested_file, "green.action") == 0)
	{
		const char* response = "r=255 g=0 b=0\r\n";
		write(lightbox_socket, response, strlen(response));
		return send_empty_response(sock_fd, http_200);
	}
	else if (strcmp(requested_file, "blue.action") == 0)
	{
		const char* response = "r=255 g=0 b=0\r\n";
		write(lightbox_socket, response, strlen(response));
		return send_empty_response(sock_fd, http_200);
	}
	else
	{
		// Worry about this later
		send_empty_response(sock_fd, http_501);
	}
}