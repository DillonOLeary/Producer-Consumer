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
    while (q->num_elem >= q->queue_size) {
        q->enqueueBlockCount = q->enqueueBlockCount + 1;
        if (0 != pthread_cond_wait(&(q->q_filling),&(q->mutex))) {
            fprintf(stderr, "Error occured waiting\n");
            exit(-1);
        }
    }
    // add string to queue at num_elem
    // increment num_elem
    q->head[q->num_elem++] = string;
    if (string[0] != EOF) q->enqueueCount++;
    // notify dequeue
    if (0 != pthread_cond_signal(&(q->q_emptying))) {
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
    while (q->num_elem <= 0) {
        q->dequeueBlockCount = q->dequeueBlockCount + 1;
        if (0 != pthread_cond_wait(&(q->q_emptying),&(q->mutex))) {
            fprintf(stderr, "Error occured waiting\n");
            exit(-1);
        }
    }
    // add string to queue at num_elem
    // increment num_elem
    ret_str = q->head[--(q->num_elem)];
    if (ret_str[0] != EOF) q->dequeueCount++;
    // notify dequeue
    if (0 != pthread_cond_signal(&(q->q_filling))) {
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
    // FIXME should this be stderr? where did I get that from...
    // FIXME enqueue and dequeue - 1 because the last element is the EOF char
    fprintf(stderr, "\tenqueueCount: %d\n\tdequeueCount: %d\n"
            "\tenqueueBlockCount: %d\n\tdequeueBlockCount: %d\n",
            q->enqueueCount, q->dequeueCount, 
            q->enqueueBlockCount, q->dequeueBlockCount);
}
