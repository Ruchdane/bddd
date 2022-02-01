#if !defined(MAINHEADEFILE)
#define MAINHEADEFILE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sda.h>
#include "db.h"
#include "network.h"
// #include "replication.h"
#include "protocole.h"
#define log(message) fprintf(stderr, "%d: %s %s\n", errno, strerror(errno), message);
struct server
{
	int socket;
	const char* port;
	const char *address;
	struct element* clients;
	struct element* peers;
};
struct client
{
	int socket;
	int readsock;
	int writesock;
	struct sockaddr_storage addr;
	pthread_t thread;
	bool isPeer;
};
void* ClientThread(void* param);
void* PeerThread(void* param);
void* broadcastDeleteThread(void* data);
void* broadcastPutThread(void* data);

#endif // MAINHEADEFILE
