#ifndef _CONN_QUEUE_H_
#define _CONN_QUEUE_H_

#include <worker.h>
#include <semaphore.h>

#define MAX_QUEUE 100

typedef struct cqueue {
    pthread_mutex_t mutex;
    sem_t slot;
    sem_t item;

    int put;
    int take;
    int queue[MAX_QUEUE];
} cqueue_t;

int cqueue_init(cqueue_t *cqueue);
int cqueue_enqueue(cqueue_t *cqueue, int value);
int cqueue_dequeue(cqueue_t *cqueue);

#endif