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
#define WEB_PORT 80						// Default web port
#define QSIZE 10						// Max amount of incoming calls to queue (arbitrary)
#define MAX_REQ_LENGTH 8192				// Maximum length for a request
#define ROOT_PATH "/webserver"			// Filepath to the preferred root directory

// Functions
int make_server_socket(int port);
int handle_call(int sock_fd);

#endif // WEBSERVER_H