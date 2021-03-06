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
#include "log.h"

enum mode {
	AP,CP,AC
};
struct server
{
	int socket;
	const char* port;
	const char *address;
	struct element* clients;
	struct element* peers;
	bool allPeerOnline;
	enum mode mode;
};
#define isWriteAllowed(server) (server.allPeerOnline || server.mode == AP)
struct client
{
	int socket;
	bool isConnected;
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
