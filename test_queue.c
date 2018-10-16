#include <stdio.h>
#include "queue.h"
#include <unistd.h>
#include <pthread.h>
#define NUM_THREADS 12
pthread_t tid[NUM_THREADS];

void * test_thread(void *q) {
    q = (Queue*)q;
    EnqueueString(q, "TEST");
    return NULL;
}

int main() {
    // FIXME why did this not work when I used fork()??
    int i;
    i = 0;
    Queue *q;
    q = CreateStringQueue(10);
    while(i < NUM_THREADS)
    {
        q = (void*)q;
        pthread_create(&(tid[i]), NULL, &test_thread, q);
        i++;
    }
    i = 0;
    while (i < NUM_THREADS) {
        pthread_join(tid[i], NULL);
        i++;
    }
    pthread_mutex_destroy(&(q->mutex));
    return 0;
}
