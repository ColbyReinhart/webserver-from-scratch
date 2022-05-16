#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

// General
#define HOSTNAME "colbyreinhart.com"
#define TEST_PORT 8080						// Port used for testing purposes
#define WEB_PORT 80							// Default web port
#define QSIZE 10							// Max amount of incoming calls to queue (arbitrary)
#define MAX_REQ_LENGTH 8192					// Maximum length for a request
#define SITES_PATH "/webserver/sites/"		// Filepath to the sites directory

// Response headers
#define HTTP_200 "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n"
#define HTTP_404 "HTTP/1.1 404 NOT FOUND\r\n\r\n"

int make_server_socket(int port);
int handle_call(int sock_fd);

#endif // WEBSERVER_H