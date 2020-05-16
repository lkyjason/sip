#ifndef _SERVER_H_
#define _SERVER_H_

#include <worker.h>

#define MAX_LISTEN_QUEUE 10
#define NUM_WORKERS 10

typedef struct server {
    worker_t workers[NUM_WORKERS];
} server_t;

#endif