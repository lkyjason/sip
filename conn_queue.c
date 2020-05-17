#include <stdio.h>
#include <string.h>
#include <conn_queue.h>

int cqueue_init(cqueue_t *cqueue) {
    if(cqueue == NULL) return -1;

    int rc;

    cqueue->put = 0;
    cqueue->take = 0;

    if( (rc = pthread_mutex_init(&(cqueue->mutex), NULL) ) != 0 ) {
        printf("[CQUEUE:MUTEX_INIT] %s\n", strerror(rc));
        return -1;
    }

    if( (rc = sem_init(&(cqueue->slot), 0, MAX_QUEUE) ) != 0) {
        perror("[CQUEUE:SEM_INIT]");
        return -1;
    }

    if( (rc = sem_init(&(cqueue->item), 0, 0) ) != 0) {
        perror("[CQUEUE:SEM_INIT]");
        return -1;
    }

    return 0;
}

int cqueue_enqueue(cqueue_t *cqueue, int value) {
    int rc;

    // attempt to get a slot
    if( (rc = sem_trywait(&(cqueue->slot)) ) != 0) {
        return -1;
    }

    // lock 
    if( (rc = pthread_mutex_lock(&(cqueue->mutex)) ) != 0 ) {
        return -1;
    }

    // enqueue
    cqueue->queue[cqueue->put] = value;
    cqueue->put = (cqueue->put + 1) % MAX_QUEUE;

    // unlock
    pthread_mutex_unlock(&(cqueue->mutex));

    // post to item semaphore
    sem_post(&(cqueue->item));

    return 0;
}

int cqueue_dequeue(cqueue_t *cqueue) {
    int rc, value;

    // attempt to get a slot
    if( (rc = sem_wait(&(cqueue->item)) ) != 0) {
        return -1;
    }

    // lock 
    if( (rc = pthread_mutex_lock(&(cqueue->mutex)) ) != 0 ) {
        return -1;
    }

    // enqueue
    value = cqueue->queue[cqueue->take];
    cqueue->take = (cqueue->take + 1) % MAX_QUEUE;

    // unlock
    pthread_mutex_unlock(&(cqueue->mutex));

    // post to item semaphore
    sem_post(&(cqueue->slot));

    return value;
}
