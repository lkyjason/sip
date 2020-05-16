#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_LISTEN_QUEUE 10


int setup_listen(char *port) {
    int listenfd, rc;
    int reuse_val = 1;
    struct addrinfo hints, *servinfo, *ptr;

    // setup hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // try to get addr info
    if( (rc = getaddrinfo(NULL, port, &hints, &servinfo) ) != 0) {
        printf("[GETADDRINFO] %s\n", gai_strerror(rc));
        return -1;
    }

    for(ptr = servinfo ; ptr != NULL ; ptr = ptr->ai_next) {
        // get listen socket
        if( (listenfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol) ) == -1 ) { 
            perror("[SERVER:SOCKET]");
            continue;
        }

        // allow resuse
        if( (rc = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse_val, sizeof(int)) ) == -1 ) {
            perror("[SERVER:SETSOCKOPT]");
            return -1;
        }

        // bind to port
        if( (rc = bind(listenfd, ptr->ai_addr, ptr->ai_addrlen) ) == -1 ) {
            perror("[SERVER:BIND]");
            continue;
        }
    }

    freeaddrinfo(servinfo);
    if(ptr == NULL) return -1;
    else return listenfd;
}


int main(int argc, char *argv[]) {
    int rc, listenfd;

    if(argc != 2) {
        printf("./sip_server <PORT>");
        exit(1);
    }

    char *port = argv[1];

    // setup listening socket
    if( (listenfd = setup_listen(port) ) == -1) {
        exit(1);
    }

    // listen
    if( (rc = listen(listenfd, MAX_LISTEN_QUEUE) ) == -1 ) {
        perror("[SERVER:LISTEN]");
        exit(1);
    }

    printf("server listening on port %s", port);

    // main accept loop
    while(1) {

    }

    return 0;
}