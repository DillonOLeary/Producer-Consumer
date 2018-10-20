#include "p2_thread.h"
#include "queue.h"
#include <stdio.h>
#include <stdbool.h>

#define END          NULL
#define a_ASCII      97
#define z_ASCII      122

const int IN_BUFF_SIZE = 1024;
const int QUEUE_SIZE = 10;

P2_thread ** CreateThreadArray() {
    P2_thread **threads = calloc(NUM_THREADS, sizeof(P2_thread *));
    if (threads == NULL) {
        printf("Error, failure to allocate memory");
        exit(1);
    }
    int i;
    
    Queue *readToMun1  = CreateStringQueue(QUEUE_SIZE);
    Queue *mun1ToMun2  = CreateStringQueue(QUEUE_SIZE);
    Queue *mun2ToWrite = CreateStringQueue(QUEUE_SIZE);

    for (i = 0; i < NUM_THREADS; i++) {
        threads[i] = calloc(1, sizeof(P2_thread));
    }
    
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
    char *input = calloc(IN_BUFF_SIZE, sizeof(char));   // Create our input buffer
    int i;
    char temp;
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        input[i] = fgetc(stdin); // Snag a character
        /* Check if the new character signals that we are done with a line */
        // FIXME we need to append a '\0' character at the end of buffer when read from stdin to show it is a String
        // unless it is already null at the end
        if (input[i] == '\n' || input[i] == EOF) {
            temp = input[i];
            input[i] = '\0';
            if (i == 0 && temp == EOF) {
                EnqueueString(t->next_q, END);
                return DONE;
                // Do nothing here
            } else {
                EnqueueString(t->next_q, input);
            }
            if (temp == EOF) {
                EnqueueString(t->next_q, END);
                return DONE;
            }
            return NOT_DONE;
        }
    }
    // FIXME this should then do some sort of flush
    // fprintf(stderr, "Input line too long\n"); 
    
    char c = getc(stdin);
    while( c != '\n' && c != EOF ) c = getc(stdin);
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
    if (string == END) {
        EnqueueString(t->next_q, string);
        return DONE;
    }
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\0') {
            EnqueueString(t->next_q, string);
            return NOT_DONE;
        }
        if (string[i] >= a_ASCII && string[i] <= z_ASCII) 
            string[i] = string[i] - 32;     // Convert the ascii value from an upper case value to lower case
    }
    return NOT_DONE;
}

int WriterAction(P2_thread *t) {
    char *string = DequeueString(t->this_q);
    if (string == END) {
        free(string);
        return DONE;
    }

    int i;
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\0') {
            printf("\n");
            free(string);
            return NOT_DONE;
        }
        printf("%c", string[i]);   
    }
    return NOT_DONE;
}

void PrintThreadQueueStats(P2_thread *t) {
    PrintQueueStats(t->this_q);
}
