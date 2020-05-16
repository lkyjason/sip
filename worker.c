#include <stdio.h>

#include <pthread.h>
#include <worker.h>

void *worker_func( void *ptr ) {
    worker_t *worker = (worker_t *) ptr;


    printf("hello i am worker %d\n", worker->worker_id);

    return 0; 
}