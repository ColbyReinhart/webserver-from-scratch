webserver: webserver.c call_handler.c utilities.c servlets/*
	cc -o webserver webserver.c utilities.c call_handler.c servlets/*

test: webserver.c call_handler.c utilities.c servlets/*
	cc -DTEST -o webserver webserver.c utilities.c call_handler.c servlets/*

clean:
	rm -rf webserver