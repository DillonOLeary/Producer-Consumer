#include "p2_thread.h"

void * run_thread(void *t) {
    while (DONE != (((P2_thread*)t)->DoAction)(t));
    // TODO use calloc to 0 out memory location
    // TODO pass termination EOF through queues
    pthread_exit(NULL);
    return NULL;
}

void run() {
    pthread_t tid[NUM_THREADS];
    P2_thread ** t_array;
    t_array = CreateThreadArray();
    int i;
    for (i = 0; i < NUM_THREADS; i++) {
        if (0 != pthread_create(&(tid[i]), NULL, &run_thread, (void*) t_array[i])) {
            fprintf(stderr, "Error creating thread\n");
            exit(-1);
        }
    }
    for (i = 0; i < NUM_THREADS; i++) {
        if (0 != pthread_join(tid[i], NULL)) {
            fprintf(stderr, "Error joining thread\n");
            exit(-1);
        }
    }

    // FIXME This assumes the first thread is the reader thread, which has no queue
    printf("\n");
    for (i = 1; i < NUM_THREADS; i++) {
        printf("Queue %d:\n", i);
        PrintThreadQueueStats(t_array[i]);
    }
}
