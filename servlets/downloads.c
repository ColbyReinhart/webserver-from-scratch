// downloads.c
// Servlet for handling file downloads
// Author: Colby Reinhart
// 5-28-2022

#include "../webserver.h"
#include <sys/stat.h>
#include <sys/types.h>

int serve_download(int sock_fd, char* requested_file)
{
	// Get content type
	char content_type[50];
	get_content_type(requested_file, content_type);

	// Contruct the filepath
	char file_path[200]; // Arbitrarily large
	strcpy(file_path, ROOT_PATH);
	strcat(file_path, "/downloads/");
	strcat(file_path, requested_file);

	// Get the file size
	struct stat st;
	if (stat(file_path, &st) != 0)
	{
		perror("Couldn't get file size of download");
		return -1;
	}

	// Send appropriate headers
	char response_text[MAX_REQ_LENGTH]; // Arbitrarily large
	sprintf
	(
		response_text,
		"HTTP/1.1 200 OK\r\nContent-type: %s\r\nContent-Disposition: attachment; filename=%s\r\nContent-Transfar-Encoding: binary\r\nAccept-Ranges: bytes\r\nContent-Length: %ld\r\n\r\n",
		content_type,
		requested_file,
		st.st_size
	);
	if (write(sock_fd, response_text, strlen(response_text)) == -1)
	{
		perror("Couldn't respond to client");
		return -1;
	}

	// Read the requested file and write it to the socket
	FILE* webpage_to_serve = fopen(file_path, "r");
	FILE* socket = fdopen(sock_fd, "w");
	char buffer[DOWNLOAD_BUFSIZ]; // Downloads can be pretty large!
	int bytes_read = 0;
	do
	{
		bytes_read = fread(buffer, 1, DOWNLOAD_BUFSIZ, webpage_to_serve);
		fwrite(buffer, 1, bytes_read, socket);
	} while (bytes_read > 0);
	fclose(webpage_to_serve);
	fclose(socket);

	return 0;
}