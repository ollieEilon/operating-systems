#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "processQueue.h"

// Size of the initial array for the queue
#define INITIAL_QUEUE_SIZE 10

// Instantiates a new process queue
ProcessQueue newQueue() {
    ProcessQueue q;
    q.size = INITIAL_QUEUE_SIZE;
    q.array = malloc(q.size * sizeof(Process));
    assert (q.array != NULL);
    q.back = 0;
    q.front = 0;
    return q;
}

// Checks whether the queue is empty
int isEmptyQueue(ProcessQueue q) {
    return (q.back == q.front);
}

// Prints an error message for the queue being empty and aborts the program
void queueEmptyError() {
    printf("queue empty \n");
    abort();
}

// Doubles the size of the queue
void doubleQueueSize(ProcessQueue *qp) {
    int i;
    int oldSize = qp->size;
    qp->size = 2 * oldSize;
    qp->array = realloc(qp->array, qp->size * sizeof(Process));
    assert (qp->array != NULL);
    for (i = 0; i < qp->back; i++) { /* eliminate split */
        qp->array[oldSize + i] = qp->array[i];
    }
    qp->back = qp->back + oldSize; /* update qp . back */
}

// Frees the memory of a queue
void freeQueue(ProcessQueue q) {
    free(q.array);
}

// Enqueues a process to the queue
void enqueue(Process item, ProcessQueue *qp) {
    qp->array[qp->back] = item;
    qp->back = (qp->back + 1) % qp->size;
    if (qp->back == qp->front) {
        doubleQueueSize(qp);
    }
}

// Dequeues a process from the queue
Process dequeue(ProcessQueue *qp) {
    Process item;
    if (isEmptyQueue(*qp)) {
        queueEmptyError();
    }
    item = qp->array[qp->front];
    qp->front = (qp->front + 1) % qp->size;
    return item;
}