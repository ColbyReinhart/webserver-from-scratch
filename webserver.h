#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

// General
#define HOSTNAME "colbyreinhart.com"
#define WEB_PORT 80						// Default web port
#define QSIZE 10						// Max amount of incoming calls to queue (arbitrary)
#define MAX_REQ_LENGTH 8192				// Maximum length for a request
#define ROOT_PATH "/webserver"			// Filepath to the preferred root directory
#define LOGFILE_PATH "/webserver/logs/"	// Filepath to the logs folder
#define LOG_MSG_SIZE 500				// Maximum size of a log message
#define ERROR_FD 2						// The file descriptor for error logging (stderr)
#define LOG_FD 3						// The file descriptor for general logging

// Structures
struct response_entry
{
	int code;
	char* message;
};

// Functions
int make_server_socket(int port);
int handle_call(int sock_fd);
int log_setup();
void cleanup_and_exit();
int is_supported_request_type(char* request_type);
int send_empty_response(int sock_fd, struct response_entry response);
int send_normal_response(int sock_fd, struct response_entry response, char* file_path);
int get_server_route(char* url_route, char* route_to_fill);

#endif // WEBSERVER_H