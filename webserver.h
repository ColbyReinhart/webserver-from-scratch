// webserver.h
// Header associated with main process
// Written by Colby Reinhart
// Revamped 8-25-2022

#ifndef WEBSERVER_H
#define WEBSERVER_H

#define MAX_REQ_LENGTH 8192		// Maximum length for a request
#define WEB_PORT 5000
#define PING_INTERVAL 900		// Every 15 minutes (900 seconds)

#ifdef TEST
#define WEB_PORT 8080
#define ROOT_PATH "/home/colbyreinhart/revamp"
#else
#define WEB_PORT 80
#define ROOT_PATH "/webserver"
#endif

#endif // WEBSERVER_H