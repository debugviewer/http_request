#include <stdio.h>
#include "http_handler.h"

handler_map g_handler;

void print_args(std::string const &name, std::string args) {
	printf("handler: %s\n", name.c_str());
	printf("args: %s\n", args.c_str());
}

int add_user(char *args) {
	if (!args) return HTTP_RESP_ADD_USER_ERROR;
	print_args("add_user", std::string(args));
	return 0;
}

int del_user(char *args) {
	if (!args) return HTTP_RESP_DEL_USER_ERROR;
	print_args("del_user", std::string(args));
	return 0;
}

int mod_user(char *args) {
	if (!args) return HTTP_RESP_MOD_USER_ERROR;
	print_args("mod_user", std::string(args));
	return 0;
}

int query_user(char *args) {
	if (!args) return HTTP_RESP_QUERY_USER_ERROR;
	print_args("query_user", std::string(args));
	return 0;
}

void add_handler(std::string const& handler_name, http_request_handler handler) {
	g_handler.insert(std::make_pair(handler_name, handler));
}

int init_http_request_handler() {
	add_handler("/add_user", add_user);
	add_handler("/del_user", del_user);
	add_handler("/mod_user", mod_user);
	add_handler("/query_user", query_user);

	return 0;
}

http_request_handler find_handler(std::string const &handler_name) {
	auto it = g_handler.find(handler_name);
	if (it == g_handler.end()) {
		printf("%s did not register handler\n", handler_name.c_str());
		return NULL;
	}

	return it->second;
}