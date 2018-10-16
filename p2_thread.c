#include "p2_threads.h"
#include <stdbool.h>

#define IN_BUFF_SIZE 128
#define a_ASCII      97
#define z_ASCII      122
#define TO_UPPER_CASE(c) (c - 32)


int ReaderAction(P2_thread *t) {
   
    char input[IN_BUFF_SIZE];
    int i;

    for (i = 0; i < IN_BUFF_SIZE; i++) {
        input[i] = fgetc(t->in);
        if (input[i] == '\n' || input[i] == EOF) {
            EnqueueString(t->next_q, input);
            return 0;
        }
    }
    return 1;   // Disregard line

}


int Munch1Action(P2_thread *t) {
    int i;
    char string[IN_BUFF_SIZE]; 
    string = DequeueString(t->this_q);
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\n' || string[i] == EOF) {
            EnqueueString(t->next_q, string);
            return 0;
        }
        if (string[i] == ' ') string[i] = '*';
    }
    return 1;
}

int Munch2Action(P2_thread *t) {
    int i;
    char string[IN_BUFF_SIZE]; 
    string = DequeueString(t->this_q);
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\n' || string[i] == EOF) {
            EnqueueString(t->next_q, string);
        }
        if (string[i] >= a_ASCII && string[i] <= z_ASCII) 
            string[i] = TO_UPPER_CASE(string[i]);
    }
    return 1;

}

int WriterAction(P2_thread *t) {
    
}
