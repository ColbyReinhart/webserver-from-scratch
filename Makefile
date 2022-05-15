webserver: webserver.c call_handler.c
	cc -o webserver webserver.c call_handler.c

clean:
	rm -rf netpong