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
    // TODO remember to free the allocation
    q->enqueueCount = q->dequeueCount = 0;
    q->enqueueBlockCount = q->dequeueBlockCount = 0;
    return q;
}

void EnqueueString(Queue *q, char *string) {
    if (0 != pthread_mutex_lock(&(q->mutex))) {
        printf("Error occured locking mutex\n");
        exit(-1);
    }
    int  i;
    printf("%s\n",string);
    for(i=0; i<(0xFFFFFFFF);i++);
    printf("Type in a number \n");
    //scanf("%d", &number);
    printf("1\n");
    printf("2\n");
    printf("3\n");
    printf("4\n");
    printf("5\n");
    printf("6\n");
    // lock with monitor
    // while the queue is full then increment blocked count and  wait
    // add string to queue at num_elem
    // increment num_elem
    // notify dequeue
    // unlock monitor
    pthread_mutex_unlock(&(q->mutex));


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
    // sem_P(queue_remove)
    // sem_P(mutex)
    // remove string from queue at index 0
    // deccrement num_elem
    // sem_V(mutex)
    // sem_V(queue_add)
    return NULL;
}

void PrintQueueStats(Queue *q) {

}
