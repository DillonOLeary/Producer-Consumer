#ifndef __queue_h__
#define __queue_h__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
/**
 * This module contains the queue stuct and
 * the functions for acting on the queue
*/

// The Queue struct contains basic info about the Queue
typedef struct {
    char ** head;
    int queue_size;
    int num_elem;
    // TODO sem mutex -- or some other lock
    pthread_mutex_t mutex;
    pthread_cond_t q_filling;
    pthread_cond_t q_emptying;
    int enqueueCount;
    int dequeueCount;
    int enqueueBlockCount;
    int dequeueBlockCount;
} Queue;

/**
 * The function returns a pointer to the new queue structure.
 */
Queue *CreateStringQueue(int size);

/**
 * This function places the pointer to the string 
 * at the end of queue q. If the queue is full, 
 * then this function blocks until there is space available. 
 */
void EnqueueString(Queue *q, char *string);

/**
 * This function removes a pointer to a string from the 
 * beginning of queue q. If the queue is empty, then 
 * this function blocks until there is a string placed 
 * into the queue. This function returns the pointer
 * that was removed from the queue.
 */
char * DequeueString(Queue *q);

/**
 * This function prints the statistics for this queue
 */
void PrintQueueStats(Queue *q);

#endif
