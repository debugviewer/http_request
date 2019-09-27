#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "reactor.h"
#include "http_parse.h"
#include "http_handler.h"

#define HTTP_HEADER_LINE_LENGTH 4096

enum {
	HTTP_RESP_SUCCESS = 0,
	HTTP_RESP_NOT_FOUND = 404,
	HTTP_RESP_SERVER_UNAVAILABLE = 503,
	HTTP_RESP_URI_NOT_FOUND = 555
};

int read_line(const char *buffer, int length, int idx, char *line) {

	for (; idx<length; idx++) {
		if(buffer[idx] == '\r' && buffer[idx+1] == '\n') {
			return idx + 2;
		} else {
			*(line++) = buffer[idx];
		}
	}

	return -1;
}

int http_response(const char *head, const char *content, char* sbuffer) {
	int length = 0;
	const char *status_line = head;
	strcpy(sbuffer, status_line);
	length = strlen(status_line);

	const char *content_type = "Content-Type: text/html;charset=utf-8\r\n";
	strcat(sbuffer, content_type);
	length += strlen(content_type);

	length += sprintf(sbuffer+length, "Content-Length: %ld\r\n", strlen(content));
	strcat(sbuffer, "\r\n");
	length += 2;

	strcpy(sbuffer+length, content);
	length+=strlen(content);

	return length;
}


// http header;
// http body

int html_request(const char *path, const char *head, char* sbuffer) {
	// http header
	struct stat st;
	if (stat(path, &st) < 0) {
		printf("stat file error \n");
		return -1;
	}
	
	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		printf("open file %s error\n", path);
		return -2;
	}
	
	char *fp = (char *)mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	int length = http_response(head, fp, sbuffer);
	close(fd);
	return length;
}

int send_errno(char *sbuffer, int err_code) {

	switch (err_code)
	{
	case HTTP_RESP_NOT_FOUND:
		return html_request("www/404.html", "HTTP/1.0 404 Not Found\r\n", sbuffer);
	case HTTP_RESP_SERVER_UNAVAILABLE:
		return html_request("www/503.html", "HTTP/1.0 Server Not Available\r\n", sbuffer);
	case HTTP_RESP_SUCCESS:
		return http_response("HTTP/1.0 200 OK\r\n", "<H1>success</H1>", sbuffer);
	case HTTP_RESP_URI_NOT_FOUND:
		return http_response("HTTP/1.0 200 OK\r\n", "<H1>uri not found</H1>", sbuffer);
	case HTTP_RESP_ADD_USER_ERROR:
		return http_response("HTTP/1.0 200 OK\r\n", "<H1>add user error</H1>", sbuffer);
	case HTTP_RESP_DEL_USER_ERROR:
		return http_response("HTTP/1.0 200 OK\r\n", "<H1>del user error</H1>", sbuffer);
	case HTTP_RESP_MOD_USER_ERROR:
		return http_response("HTTP/1.0 200 OK\r\n", "<H1>mod user error</H1>", sbuffer);
	case HTTP_RESP_QUERY_USER_ERROR:
		return http_response("HTTP/1.0 200 OK\r\n", "<H1>query user error</H1>", sbuffer);
	}
	return 0;
}

int http_handler(void *arg) {

	struct ntyevent *ev = (struct ntyevent*)arg;
	char *rbuffer = ev->rbuffer;
	int rlength = ev->rlength;


	//1 GET / HTTP/1.0

	int idx = 0;
	char line[HTTP_HEADER_LINE_LENGTH] = {0};
	if (read_line(rbuffer, rlength, idx, line) < 0) {
		// send 500 err code
		ev->slength = send_errno(ev->sbuffer, HTTP_RESP_NOT_FOUND);
	}

	//printf("%s\n", line);

	struct http_request_line request = {0};
	parse_http_request_line(line, &request);
 	print_http_request_line(&request);

	http_request_handler _handler = find_handler(request.url);
	int ret = 0;
	if (_handler) {
		ret = _handler(request.query_string);
	}
	else {
		ret = HTTP_RESP_URI_NOT_FOUND;
	}
	
	ev->slength = send_errno(ev->sbuffer, ret);

	//char *sbuffer = ev->sbuffer;

	return 0;

}


int main() {

	init_http_request_handler();
	reactor_setup(http_handler);

}





