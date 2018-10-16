#include <stdio.h>
#include "queue.h"
#include <unistd.h>
#include <pthread.h>
#define NUM_THREADS 30
pthread_t tid[NUM_THREADS];

void * test_thread(void *q) {
    q = (Queue*)q;
    int i;
    for (i = 0; i < 300000; i++)
        EnqueueString(q, "TEST");
    return NULL;
}

void * test_thread1(void *q) {
    q = (Queue*)q;
    int i;
    for (i = 0; i < 300000; i++)
        DequeueString(q);
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
        if (i < NUM_THREADS / 2) {
            pthread_create(&(tid[i]), NULL, &test_thread1, q);
        } else {
            pthread_create(&(tid[i]), NULL, &test_thread, q);
        }
        i++;
    }
    i = 0;
    while (i < NUM_THREADS) {
        pthread_join(tid[i], NULL);
        i++;
    }
    pthread_mutex_destroy(&(q->mutex));
    PrintQueueStats(q);
    return 0;
}
