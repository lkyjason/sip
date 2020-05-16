#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_LISTEN_QUEUE 10

void print_addr(struct addrinfo *p) {

    char ipstr[INET6_ADDRSTRLEN];
    char *ipver;
    void *addr;

    if (p->ai_family == AF_INET) { // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);
        ipver = "IPv4";
    } else { // IPv6
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
        addr = &(ipv6->sin6_addr);
        ipver = "IPv6";
    }

    inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
    printf("addr is %s %s\n", ipver, ipstr);
}

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
        // print info
        print_addr(ptr);

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

        break;
    }

    if(ptr == NULL) listenfd = -1;

    freeaddrinfo(servinfo);
    return listenfd;
}


int main(int argc, char *argv[]) {
    int rc, listenfd;

    if(argc != 2) {
        printf("./sip_server <PORT>\n");
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

    printf("server listening on port %s\n", port);

    struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof(their_addr);
    int clientfd;

    // main accept loop
    while(1) {
        if( (clientfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size) ) == -1 ) {
            perror("[SERVER:ACCEPT]");
            continue;
        }

        printf("hello world...\n");
        fflush(stdout);

        // print_addr((struct sockaddr *) &their_addr);

        close(clientfd);
    }

    return 0;
}