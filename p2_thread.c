#include "p2_thread.h"
#include "queue.h"
#include <stdio.h>
#include <stdbool.h>

#define a_ASCII      97
#define z_ASCII      122

const int IN_BUFF_SIZE = 1024;

P2_thread ** CreateThreadArray() {
    P2_thread **threads = calloc(NUM_THREADS, sizeof(P2_thread *));
    if (threads == NULL) {
        printf("Error, failure to allocate memory");
        exit(1);
    }
    int i;
    
    Queue *readToMun1  = CreateStringQueue(IN_BUFF_SIZE);
    Queue *mun1ToMun2  = CreateStringQueue(IN_BUFF_SIZE);
    Queue *mun2ToWrite = CreateStringQueue(IN_BUFF_SIZE);

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
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        input[i] = getc(stdin); // Snag a character
        /* Check if the new character signals that we are done with a line */
        if (input[i] == '\n' || input[i] == EOF) {
            if (input[i] == EOF) return DONE;
            EnqueueString(t->next_q, input);
            else return NOT_DONE;
        } else continue;
    }
    return NOT_DONE;   // Disregard line

}


int Munch1Action(P2_thread *t) {
    int i;
    char *string; 
    string = DequeueString(t->this_q);
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\n' || string[i] == EOF) {
            EnqueueString(t->next_q, string);
            if (string[i] == EOF) return DONE;
            else return NOT_DONE;
        }
        if (string[i] == ' ') string[i] = '*';
    }
    return NOT_DONE;
}

int Munch2Action(P2_thread *t) {
    int i;
    char *string; 
    string = DequeueString(t->this_q);
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\n' || string[i] == EOF) {
            EnqueueString(t->next_q, string);
            if (string[i] == EOF) return DONE;
            else return NOT_DONE;

        }
        if (string[i] >= a_ASCII && string[i] <= z_ASCII) 
            string[i] = string[i] - 32;     // Convert the ascii value from an upper case value to lower case
    }
    return NOT_DONE;

}

int WriterAction(P2_thread *t) {
    char *string = DequeueString(t->this_q);
    int i;
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\n' || string[i] == EOF) {
            printf("\n");
            if (string[i] == EOF) return DONE;
            else {
                free(string);
                return NOT_DONE;
            }
        }
        printf("%c", string[i]);   
    }
    return NOT_DONE;
}

void PrintThreadQueueStats(P2_thread *t) {
    PrintQueueStats(t->this_q);
}
