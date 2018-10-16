#include "p2_threads.h"
#include <stdbool.h>

#define a_ASCII      97
#define z_ASCII      122
#define TO_UPPER_CASE(c) (c - 32)

const int IN_BUFF_SIZE = 1024;

int ReaderAction(P2_thread *t) {
   
    char *input = malloc(sizeof(char) * IN_BUFF_SIZE);
    int i;

    for (i = 0; i < IN_BUFF_SIZE; i++) {
        input[i] = fgetc(t->in);
        if (input[i] == '\n' || input[i] == EOF) {
            EnqueueString(t->next_q, input);
            if (input[i] == EOF) return DONE;
            else return NOT_DONE;
        }
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
            if (input[i] == EOF) return DONE;
            else return NOT_DONE;
        }
        if (string[i] == ' ') string[i] = '*';
    }
    return 1;
}

int Munch2Action(P2_thread *t) {
    int i;
    char *string; 
    string = DequeueString(t->this_q);
    for (i = 0; i < IN_BUFF_SIZE; i++) {
        if (string[i] == '\n' || string[i] == EOF) {
            EnqueueString(t->next_q, string);
            if (input[i] == EOF) return DONE;
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
            printf("%s", string);
            if (input[i] == EOF) return DONE;
            else {
                free(string);
                return NOT_DONE;
            }
        }   
    }
}
