#include <stdio.h>
#include "http_parse.h"

int parse_uri(char *uri, char **url, char **query_string) {
	char *start = uri;
	*url = start;
	for (; *start; start++)	{
		if(*start == '?') {
			*start = '\0';
			*query_string = start + 1;
			break;
		}
	}

	return 0;
}

int parse_http_request_line(char *line, struct http_request_line *request) {
	char *start = line;
	char *method = start;
	char *uri = NULL;
	char *url = NULL;
	char *query_string = NULL;
	char *version = NULL;
	for (; *start; start++)	{
		if(*start == ' ') {
			if(uri == NULL) {
				uri = start + 1;
			} else {
				version = start + 1;
			}

			*start = '\0';
		}
	}
	
	if (uri) {
		parse_uri(uri, &url, &query_string);
	}

	request->method = method;
	request->url = url;
	request->query_string = query_string;
	request->version = version;
	return 0;
}


void print_http_request_line(struct http_request_line *request) {
	if (!request) return;
	if (request->method) {
		printf("method: %s\n", request->method);
	}
	
	if (request->url) {
		printf("url: %s\n", request->url);
	}
	
	if (request->query_string) {
		printf("query_string: %s\n", request->query_string);
	}
	
	if (request->version) {
		printf("version: %s\n", request->version);
	}
}

#if 0
#define HTTP_REQUEST_LINE "GET /add_user?name=zhangsan&age=20 HTTP/1.1"
int main()
{
	init_http_request_handler();

	struct http_request_line request;
	int ret = parse_http_request_line(HTTP_REQUEST_LINE, &request);
	if (ret != SUCCESS) {
		printf("parse http header error\n");
		return ret;
	}

 	print_http_request_line(&request);

	auto _handler = find_handler(request.url);
	if (_handler) {
		_handler(request.uri.args);
	}
	
    return 0;
}
#endif
