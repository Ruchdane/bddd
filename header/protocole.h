#if !defined(NETWORKCLINETSIDE)
#define NETWORKCLINETSIDE

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
#include <stdbool.h>

int _put(int sock,char *key ,size_t keyLen, char *value, size_t valueLen);

char *_get(int sock,char *key ,size_t keyLen);

int _delete(int sock,char *key ,size_t keyLen);

ssize_t __get(int sock, char *(*get)(char *));

char * __delete(int sock, int (*delete)(char *));

struct json * __put(int sock, struct json * (*put)(char *,char *));

#define networkWrite(sock,ptr,len) if(write(sock,ptr,len) != len) printf("Number of bytes writen doesn't corespond")
#define networkRead(sock,ptr,len) if(read(sock,ptr,len) != len) printf("Number of bytes read doesn't corespond")
#endif // NETWORKCLINETSIDE
