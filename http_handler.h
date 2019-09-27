#ifndef __HTTP_HANDLER_H__
#define __HTTP_HANDLER_H__
#include <string>
#include <map>

enum USER_ERROR{
	HTTP_RESP_ADD_USER_ERROR = 1001,
	HTTP_RESP_DEL_USER_ERROR = 1002,
	HTTP_RESP_MOD_USER_ERROR = 1003,
	HTTP_RESP_QUERY_USER_ERROR = 1004
};

typedef int(*http_request_handler)(char *args);
typedef std::map<const std::string, http_request_handler> handler_map;

int init_http_request_handler();
void add_handler(std::string const& handler_name, http_request_handler handler);
http_request_handler find_handler(std::string const &handler_name);

#endif
