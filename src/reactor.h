#ifndef __REACTOR_H__
#define __REACTOR_H__

#define BUFFER_LENGTH		4096
#define MAX_EPOLL_EVENTS	1024
#define SERVER_PORT			8888

typedef int NCALLBACK(int fd, int events, void *arg);
typedef int NHANDLER(void *arg);


struct ntyevent {
	int fd;
	int events;
	void *arg;
	int (*callback)(int fd, int events, void *arg);
	
	int status;
	
	char rbuffer[BUFFER_LENGTH];
	int rlength;

	char sbuffer[BUFFER_LENGTH];
	int slength;
	long last_active;
};

struct ntyreactor {
	int epfd;
	struct ntyevent *events;
	NHANDLER *handler;
};

int reactor_setup(NHANDLER *handler);


#endif


