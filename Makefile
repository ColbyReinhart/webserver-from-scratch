webserver: webserver.c call_handler.c utilities.c servlets/homepage.c
	cc -o webserver webserver.c utilities.c call_handler.c servlets/homepage.c

clean:
	rm -rf webserver