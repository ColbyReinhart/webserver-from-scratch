// utilities.h
// Header for miscellaneous utility functions
// Written by Colby Reinhart
// Revamped 8-25-2022

#ifndef UTILITIES_H
#define UTILITIES_H

#define QSIZE 10	// Max amount of incoming calls to queue (arbitrary)
#define MAX_ROUTE_LENGTH 1000

#ifdef TEST
#define WEB_PORT 8080
#else
#define WEB_PORT 80
#endif

enum req_type { GET, POST, PUT, DELETE, CONNECT, HEAD, OPTIONS, TRACE, PATCH };

struct client_req
{
	enum req_type type;
	char route[MAX_ROUTE_LENGTH];
	char** headers;
	char* body;
}

#endif // UTILITIES_H