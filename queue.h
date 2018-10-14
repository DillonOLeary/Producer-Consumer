/**
 * This module contains the queue stuct and
 * the functions for acting on the queue
*/

// The Queue struct contains basic info about the Queue
typedef struct {
    char ** head;
    char ** tail;
    // TODO sem mutex -- or some other lock
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
