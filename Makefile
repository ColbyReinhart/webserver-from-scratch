webserver: webserver.c call_handler.c utilities.c servlets/homepage.c servlets/downloads.c
	cc -o webserver webserver.c utilities.c call_handler.c servlets/homepage.c servlets/downloads.c

clean:
	rm -rf webserver