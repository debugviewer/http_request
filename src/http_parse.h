#ifndef __HTTP_PARSE_H__
#define __HTTP_PARSE_H__

struct http_request_line {
	char *method;
	char *url;
	char *query_string;
	char *version;
	char *body;
};

int parse_http_request_line(char *line, struct http_request_line *request);

void print_http_request_line(struct http_request_line *request);

#endif
