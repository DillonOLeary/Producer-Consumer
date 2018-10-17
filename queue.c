#include "queue.h"
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>

Queue *CreateStringQueue(int size) {
    Queue *q;
    q = malloc(sizeof(Queue));
    q->queue_size = size;
    q->num_elem = 0;
    q->head = malloc(q->queue_size * sizeof(char*));
    if (0 != pthread_mutex_init(&(q->mutex), NULL) ) {
        printf("Error initializing mutex\n");
        exit(-1);
    }
    if (0 != pthread_cond_init(&(q->q_filling), NULL) ) {
        printf("Error initializing condition var filling\n");
        exit(-1);
    }
    if (0 != pthread_cond_init(&(q->q_emptying), NULL) ) {
        printf("Error initializing condition var emptying\n");
        exit(-1);
    }
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
    // while the queue is full then increment blocked count and  wait
    while (q->num_elem >= q->queue_size) {
        q->enqueueBlockCount = q->enqueueBlockCount + 1;
        if (0 != pthread_cond_wait(&(q->q_filling),&(q->mutex))) {
            printf("Error occured waiting\n");
            exit(-1);
        }
    }
    // add string to queue at num_elem
    // increment num_elem
    q->head[q->num_elem++] = string;
    q->enqueueCount++;
    // notify dequeue
    if (0 != pthread_cond_signal(&(q->q_emptying))) {
        printf("Error occured signaling condition var\n");
        exit(-1);
    }
    // unlock monitor
    if (0 != pthread_mutex_unlock(&(q->mutex))) {
        printf("Error occured unlocking mutex\n");
        exit(-1);
    }
}

char * DequeueString(Queue *q) {
    if (0 != pthread_mutex_lock(&(q->mutex))) {
        printf("Error occured locking mutex\n");
        exit(-1);
    }
    char* ret_str;
    // while the queue is full then increment blocked count and  wait
    while (q->num_elem <= 0) {
        q->dequeueBlockCount = q->dequeueBlockCount + 1;
        if (0 != pthread_cond_wait(&(q->q_emptying),&(q->mutex))) {
            printf("Error occured waiting\n");
            exit(-1);
        }
    }
    // add string to queue at num_elem
    // increment num_elem
    ret_str = q->head[--(q->num_elem)];
    q->dequeueCount++;
    // notify dequeue
    if (0 != pthread_cond_signal(&(q->q_filling))) {
        printf("Error occured signaling condition var\n");
        exit(-1);
    }

    if (0 != pthread_mutex_unlock(&(q->mutex))) {
        printf("Error occured unlocking mutex\n");
        exit(-1);
    }
    return ret_str;
}

void PrintQueueStats(Queue *q) {
    if (q == NULL) return;
    fprintf(stderr, "enqueueCount: %d, dequeueCount: %d, "
            "enqueueBlockCount: %d, dequeueBlockCount: %d\n",
            q->enqueueCount, q->dequeueCount, 
            q->enqueueBlockCount, q->dequeueBlockCount);
}
