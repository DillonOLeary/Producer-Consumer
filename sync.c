#define NUM_THREADS 4
#include "P2_thread.h"

void * run_thread(P2_thread *t) {
    // FIXME i don't know if this is the correct way to call these functions
    while (DONE != (t->DoAction)(t));
    return NULL;
}

void run() {
    pthread_t tid[NUM_THREADS];
    P2_thread ** t_array;
    t_array = CreateThreadArray();
    int i;
    for (i = 0; i < NUM_THREADS; i++) {
        // TODO check return
        pthread_create(&(tid[i]), NULL, &run_thread, t_array[i]);
    }
    for (i = 0; i < NUM_THREADS; i++) {
        //TODO check return
        pthread_join(tid[i], NULL);
    }
}
