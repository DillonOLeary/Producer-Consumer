#ifndef __P2__thread_H__
#define __P2__thread_H__
#include "queue.h"
#include <stdbool.h>
/**
 * This module contains the p2_thread struct,
 * the p2_threads_wrapper struct,
 * and all functions needed by the different
 * do_action functions of the structures.
 */
// When there is no work left to do for a thread
#define DONE 0
#define NOT_DONE 1
#define NUM_THREADS 4
// This struct is the information about a 
// thread that will run
typedef struct {
    int (*DoAction)();
    Queue *this_q;
    Queue *next_q;
    bool readerDone;
    bool *isDone;
    // This is used by the first thread to take input
    // from stdin or another file
} P2_thread;

/**
 * Creates an array containing all the thread 
 * info stucts.
 *
 * This will create the three queues that are shared
 * by the threads
 */
P2_thread ** CreateThreadArray();

/**
 * The Reader thread will read from standard input, 
 * one line at a time. Reader will take the each 
 * line of the input and pass it to thread Munch1 
 * though a queue of character strings
 */
int ReaderAction(P2_thread *t);

/**
 * Munch1 will scan the line and replace each space
 * character (not tabs or newlines) with an asterisk
 * ("*") character. It will then pass the line to
 * thread Munch2 through another queue of character strings.
 */
int Munch1Action(P2_thread *t);

/**
 * Munch2 will scan the line and convert all lower case 
 * letters to upper case (e.g., convert "a" to "A").
 * It will then pass the line to thread Writer though
 * yet another queue of character strings.
 */
int Munch2Action(P2_thread *t);

/**
 * Writer will write the line to standard output.
 */
int WriterAction(P2_thread *t);

/**
 * Prints this threads queue stats if it has
 * a queue
 */
void PrintThreadQueueStats(P2_thread *t);

#endif
