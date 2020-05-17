#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <pthread.h>
#include <worker.h>
#include <conn_queue.h>

extern cqueue_t cqueue;

void *worker_func( void *ptr ) {
    worker_t *worker = (worker_t *) ptr;

    printf("hello i am worker %d\n", worker->worker_id);

    int clientfd, rc;
    while(1) {
        clientfd = cqueue_dequeue(&cqueue);
        if(clientfd == -1) continue;

        char msg[100];
        rc = recv(clientfd, msg, 100, 0);
        (void)(rc);
        printf("recv: %s\n", msg);

        char *ret = "yeppp";
        rc = send(clientfd, ret, 5, 0);
        (void)(rc);

    }

    return 0; 
}