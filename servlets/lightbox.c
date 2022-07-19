// lightbox.c
// Servlet for the lightbox control page
// Author: Colby Reinhart
// 7-18-2022

#include "../webserver.h"

int serve_lightbox(int sock_fd, char* requested_file)
{
	char file_path[200];
	strcpy(file_path, ROOT_PATH);
	strcat(file_path, "/website/lightbox/");
	strcat(file_path, requested_file);
	return send_normal_response(sock_fd, http_200, file_path);
}