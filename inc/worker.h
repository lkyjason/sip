#ifndef _WORKER_H_
#define _WORKER_H_

#include <pthread.h>

typedef struct worker {
    int worker_id;
    pthread_t thread;
} worker_t;

void *worker_func( void *ptr );

#endif