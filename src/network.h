#if !defined(NETWORKHEADERFILE)
#define NETWORKHEADERFILE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/ftp.h>
#include <netdb.h>

int connectTo(const char* port,const char *adr);
int bindTo(const char* port,const char *adr);
#define log(message) fprintf(stderr, "%d: %s %s\n", errno, strerror(errno), message);

#endif // NETWORKHEADERFILE
