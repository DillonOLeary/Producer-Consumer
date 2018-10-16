#include "queue.h"
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>

Queue *CreateStringQueue(int size) {
    Queue *q;
    q = malloc(sizeof(Queue));
    // TODO remember to free the queue
    q->queue_size = size;
    q->num_elem = 0;
    q->head = malloc(q->queue_size * sizeof(char*));
    if (0 != pthread_mutex_init(&(q->mutex), NULL) ) {
        printf("Error initializing mutex\n");
        exit(-1);
    }
    if (0 != pthread_cond_init(&(q->q_fulling), NULL) ) {
        printf("Error initializing condition var filling\n");
        exit(-1);
    }
    if (0 != pthread_cond_init(&(q->q_emptying), NULL) ) {
        printf("Error initializing condition var emptying\n");
        exit(-1);
    }
    // TODO remember to free the allocation
    q->enqueueCount = q->dequeueCount = 0;
    q->enqueueBlockCount = q->dequeueBlockCount = 0;
    return q;
}

void EnqueueString(Queue *q, char *string) {
    // lock with monitor
    if (0 != pthread_mutex_lock(&(q->mutex))) {
        printf("Error occured locking mutex\n");
        exit(-1);
    }
    int  i;
    printf("%s\n",string);
    for(i=0; i<(0xFFFFFFFF);i++);
    printf("End Loop\n");
    // while the queue is full then increment blocked count and  wait
    if (q->num_elem == q->queue_size) {

    }
    // add string to queue at num_elem
    // increment num_elem
    // notify dequeue
    // unlock monitor
    if (0 != pthread_mutex_unlock(&(q->mutex))) {
        printf("Error occured unlocking mutex\n");
        exit(-1);
    }

    //
    // or
    //
    // sem_P(queue_add)
    // sem_P(mutex)
    // add string to queue at num_elem
    // increment num_elem
    // sem_V(mutex)
    // sem_V(queue_remove)
}

char * DequeueString(Queue *q) {
    if (0 != pthread_mutex_lock(&(q->mutex))) {
        printf("Error occured locking mutex\n");
        exit(-1);
    }

    // sem_P(queue_remove)
    // sem_P(mutex)
    // remove string from queue at index 0
    // deccrement num_elem
    // sem_V(mutex)
    // sem_V(queue_add)
    if (0 != pthread_mutex_unlock(&(q->mutex))) {
        printf("Error occured unlocking mutex\n");
        exit(-1);
    }
    return NULL;
}

void PrintQueueStats(Queue *q) {

}
