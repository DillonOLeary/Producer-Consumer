#include "queue.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>

Queue *CreateStringQueue(int size) {
    Queue *q;
    q = calloc(1, sizeof(Queue));
    q->queue_size = size;
    q->num_elem = 0;
    q->front = q->rear = 0;
    q->head = calloc(q->queue_size, sizeof(char*));
    if (0 != pthread_mutex_init(&(q->mutex), NULL) ) {
        fprintf(stderr, "Error initializing mutex\n");
        exit(-1);
    }
    if (0 != pthread_cond_init(&(q->q_filling), NULL) ) {
        fprintf(stderr, "Error initializing condition var filling\n");
        exit(-1);
    }
    if (0 != pthread_cond_init(&(q->q_emptying), NULL) ) {
        fprintf(stderr, "Error initializing condition var emptying\n");
        exit(-1);
    }
    q->enqueueCount = q->dequeueCount = 0;
    q->enqueueBlockCount = q->dequeueBlockCount = 0;
    return q;
}


void EnqueueString(Queue *q, char *string) {
    // lock with monitor
    if (0 != pthread_mutex_lock(&(q->mutex))) {
        fprintf(stderr, "Error occured locking mutex\n");
        exit(-1);
    }
    // while the queue is full then increment blocked count and  wait
    while (((q->rear == q->queue_size - 1) && q->front == 0) || q->rear == q->front - 1) {
        q->enqueueBlockCount = q->enqueueBlockCount + 1;
        if (0 != pthread_cond_wait(&(q->q_filling),&(q->mutex))) {
            fprintf(stderr, "Error occured waiting\n");
            exit(-1);
        }
    }
    // add string to queue at num_elem
    // increment num_elem
    //q->head[q->num_elem++] = string;
    q->head[q->rear] = string;
    q->rear = q->rear + 1 == q->queue_size ? 0 : q->rear + 1;
    q->num_elem = q->num_elem + 1;
    if (string != NULL) q->enqueueCount++;
    // notify dequeue
    if (0 != pthread_cond_broadcast(&(q->q_emptying))) {
        fprintf(stderr, "Error occured signaling condition var\n");
        exit(-1);
    }
    // unlock monitor
    if (0 != pthread_mutex_unlock(&(q->mutex))) {
        fprintf(stderr, "Error occured unlocking mutex\n");
        exit(-1);
    }
    
}
char * DequeueString(Queue *q) {
    if (0 != pthread_mutex_lock(&(q->mutex))) {
        fprintf(stderr, "Error occured locking mutex\n");
        exit(-1);
    }
    char* ret_str;
    // while the queue is full then increment blocked count and  wait
    while (q->front == q->rear) {
        q->dequeueBlockCount = q->dequeueBlockCount + 1;
        if (0 != pthread_cond_wait(&(q->q_emptying),&(q->mutex))) {
            fprintf(stderr, "Error occured waiting\n");
            exit(-1);
        }
    }
    // add string to queue at num_elem
    // increment num_elem
    //q->num_elem = q->num_elem - 1;
    //ret_str = q->head[q->num_elem];
    ret_str = q->head[q->front];
    q->front = (q->front + 1 == q->queue_size) ? 0 : q->front + 1;
    //if (q->front == q->rear) q->front = q->rear = -1;
    q->num_elem = q->num_elem - 1;
    
    if (ret_str != NULL) q->dequeueCount++;
    // notify dequeue
    if (0 != pthread_cond_broadcast(&(q->q_filling))) {
        fprintf(stderr, "Error occured signaling condition var\n");
        exit(-1);
    }

    if (0 != pthread_mutex_unlock(&(q->mutex))) {
        fprintf(stderr, "Error occured unlocking mutex\n");
        exit(-1);
    }
    
    return ret_str;
}

void PrintQueueStats(Queue *q) {
    if (q == NULL) return;
    fprintf(stdout, "\tenqueueCount: %d\n\tdequeueCount: %d\n"
            "\tenqueueBlockCount: %d\n\tdequeueBlockCount: %d\n",
            q->enqueueCount, q->dequeueCount, 
            q->enqueueBlockCount, q->dequeueBlockCount);
}
