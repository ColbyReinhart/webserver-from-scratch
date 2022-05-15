webserver: webserver.c call_handler.c utilities.c
	cc -o webserver webserver.c utilities.c call_handler.c

clean:
	rm -rf netpong