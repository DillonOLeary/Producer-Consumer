#include "queue.h"
#include <stdlib.h>

Queue *CreateStringQueue(int size) {
    Queue *q;
    q = malloc(sizeof(Queue));
    // TODO remember to free the queue
    q->queue_size = size;
    q->num_elem = 0;
    q->head = malloc(q->queue_size * sizeof(char*));
    // TODO remember to free the allocation
    q->enqueueCount = q->dequeueCount = 0;
    q->enqueueBlockCount = q->dequeueBlockCount = 0;
    return q;
}

void EnqueueString(Queue *q, char *string) {
    // lock with monitor
    // while the queue is full then increment blocked count and  wait
    // add string to queue at num_elem
    // increment num_elem
    // notify dequeue
    // unlock monitor
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
