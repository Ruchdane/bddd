#include "network.h"

int connectTo(const char* port,const char *adr){
    int sock,connection;
    struct addrinfo hints,*addr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if(getaddrinfo(adr ? adr:INADDR_ANY,port,&hints,&addr)){
       log("getaddrinfo\n");
        return -1;
    }
        
    sock = socket(addr->ai_family,addr->ai_socktype,addr->ai_protocol);
    if(sock == -1){
		freeaddrinfo(addr);
        log("socket\n");
        return -1;
    }
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,NULL,SOCK_STREAM);
    connection = connect(sock,addr->ai_addr,addr->ai_addrlen);
    if(connection){
		freeaddrinfo(addr);
        close(sock);
        log("connect\n");
        return -1;
    }
    return sock;
}

int bindTo(const char* port,const char *adr){
    int sock,connection;
    struct addrinfo hints,*addr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if(getaddrinfo(adr ? adr:INADDR_ANY,port,&hints,&addr)){
       log("getaddrinfo\n");
        return -1;
    }
        
    sock = socket(addr->ai_family,addr->ai_socktype,addr->ai_protocol);
    if(sock == -1){
		freeaddrinfo(addr);
        log("socket\n");
        return -1;
    }
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,NULL,SOCK_STREAM);
    connection = bind(sock,addr->ai_addr,addr->ai_addrlen);
    if(connection){
		freeaddrinfo(addr);
        close(sock);
        log("connect\n");
        return -1;
    }
    return sock;
}