#include "p2_thread.h"
#include "queue.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define END          NULL

const int IN_BUFF_SIZE = 1024;
const int QUEUE_SIZE = 10;

P2_thread ** CreateThreadArray() {
    P2_thread **threads = calloc(NUM_THREADS, sizeof(P2_thread *));
    if (threads == NULL) {
        printf("Error, failure to allocate memory");
        exit(1);
    }
    int i;
    
    // Create the queues that the threads communicate through
    Queue *readToMun1  = CreateStringQueue(QUEUE_SIZE);
    Queue *mun1ToMun2  = CreateStringQueue(QUEUE_SIZE);
    Queue *mun2ToWrite = CreateStringQueue(QUEUE_SIZE);

    // Allocate memory for each of the threads
    for (i = 0; i < NUM_THREADS; i++) {
        threads[i] = calloc(1, sizeof(P2_thread));
        if (threads[i] == NULL) {
            fprintf(stderr, "Error, failure to allocate memory");
            exit(1);
        }
    }
    
    /* Initalize the threads with their function and queues */
    threads[0]->next_q = readToMun1;
    threads[0]->DoAction = ReaderAction;
    
    threads[1]->this_q = readToMun1;
    threads[1]->next_q = mun1ToMun2;
    threads[1]->DoAction = Munch1Action;

    threads[2]->this_q = mun1ToMun2;
    threads[2]->next_q = mun2ToWrite;
    threads[2]->DoAction = Munch2Action;

    threads[3]->this_q = mun2ToWrite;
    threads[3]->DoAction = WriterAction;
    
    return threads;
}

int ReaderAction(P2_thread *t) {
    char *input;
    if (NULL == (input = calloc(IN_BUFF_SIZE, sizeof(char)))){
        fprintf(stderr, "ERROR");
        exit(-1);   // Create our input buffer
    }
    int i;
    char temp;
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        input[i] = fgetc(stdin); // Snag a character
        /* Check if the new character signals that we are done with a line */
        // unless it is already null at the end
        if (input[i] == '\0') fprintf(stderr, "ERROR!");
        if (input[i] == '\n' || input[i] == EOF) {
            temp = input[i];
            input[i] = '\0';
            if (i == 0 && temp == EOF) { // Dont queue an unneeded empty line
                EnqueueString(t->next_q, END);
                return DONE;
                // Do nothing here
            } else {    // Even if its EOF we want the chars before that
                EnqueueString(t->next_q, input);
            }
            if (temp == EOF) {  // If we're done, tell the rest of the queues
                EnqueueString(t->next_q, END); 
                return DONE;
            }
            return NOT_DONE;
        }
    }
    
    char c = getc(stdin);
    while( c != '\n' && c != EOF ) c = getc(stdin); // discard until we're \n
    fprintf(stderr, "LINE TOO LONG\n");
    if (c == EOF) {
        EnqueueString(t->next_q, END);
        return DONE;
    }
    return NOT_DONE;   // Disregard line

}
// This should end if reader is done
int Munch1Action(P2_thread *t) {
    int i;
    char *string; 
    string = DequeueString(t->this_q);
    if (string == END) {
        EnqueueString(t->next_q, string);
        return DONE;
    }

    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\0') {
            EnqueueString(t->next_q, string);
            return NOT_DONE;
        }
        if (string[i] == ' ') string[i] = '*';
    }
    return NOT_DONE;
}

int Munch2Action(P2_thread *t) {
    int i;
    char *string; 

    string = DequeueString(t->this_q);
    // If it gets the indicator that the read is done we pass it along and end
    if (string == END) {
        EnqueueString(t->next_q, string);
        return DONE;
    }
    // Go through each char in the string and convert to upper case
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\0') {
            EnqueueString(t->next_q, string);
            return NOT_DONE;
        }
        string[i] = toupper((unsigned char) string[i]);
    }
    return NOT_DONE;
}

int WriterAction(P2_thread *t) {
    char *string = DequeueString(t->this_q);
    // Shouldn't print if we get the end indicator
    if (string == END) {
        return DONE;
    }

    int i;
    // Print each char and then when the end is reached insert a new line
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\0') {
            printf("\n");
            if (string != NULL) free(string);
            return NOT_DONE;
        }
        printf("%c", string[i]);   
    }
    free(string);
    return NOT_DONE;
}

void PrintThreadQueueStats(P2_thread *t) {
    PrintQueueStats(t->this_q);
}
