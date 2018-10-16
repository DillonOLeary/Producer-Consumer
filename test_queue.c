#include <stdio.h>
#include "queue.h"
#include <unistd.h>
#include <pthread.h>

pthread_t tid[2];

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
    while(i < 2)
    {
        q = (void*)q;
        pthread_create(&(tid[i]), NULL, &test_thread, q);
        i++;
    }
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&(q->mutex));
    return 0;
}
