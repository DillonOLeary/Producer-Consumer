#include "p2_thread.h"

void * run_thread(void *t) {
    while (DONE != (((P2_thread*)t)->DoAction)(t));
    // TODO use pthread_exit and calloc to 0 out memory location
    return NULL;
}

void run() {
    pthread_t tid[NUM_THREADS];
    P2_thread ** t_array;
    t_array = CreateThreadArray();
    int i;
    for (i = 0; i < NUM_THREADS; i++) {
        if (0 != pthread_create(&(tid[i]), NULL, &run_thread, (void*) t_array[i])) {
            printf("Error creating thread\n");
            exit(-1);
        }
    }
    for (i = 0; i < NUM_THREADS; i++) {
        if (0 != pthread_join(tid[i], NULL)) {
            printf("Error joining thread\n");
            exit(-1);
        }
    }
}
