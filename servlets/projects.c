// projects.c
// Servlet for the projects page
// Author: Colby Reinhart
// 5-28-2022

#include "../webserver.h"

int serve_projects(int sock_fd, char* requested_file)
{
	char file_path[200];
	strcpy(file_path, ROOT_PATH);
	strcat(file_path, "/website/projects/");
	strcat(file_path, requested_file);
	return send_normal_response(sock_fd, http_200, file_path);
}