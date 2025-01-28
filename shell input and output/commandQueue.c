#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "commandQueue.h"

// Size of the initial array for the queue
#define INITIAL_QUEUE_SIZE 10


// Instantiates a new process queue
CommandQueue newQueue() {
    CommandQueue q;
    q.size = INITIAL_QUEUE_SIZE;
    q.array = malloc(q.size * sizeof(Command));
    assert (q.array != NULL);
    q.back = 0;
    q.front = 0;
    return q;
}

// Checks whether the queue is empty
bool isEmptyQueue(CommandQueue q) {
    return (q.back == q.front);
}

int getQueueSize(CommandQueue q) {
    if (q.back >= q.front) {
        return (q.back - q.front);
    }
    return (q.size - q.front + q.back);
}

// Prints an error message for the queue being empty and aborts the program
void queueEmptyError() {
    printf("queue empty \n");
    abort();
}

// Doubles the size of the queue
void doubleQueueSize(CommandQueue *qp) {
    int i;
    int oldSize = qp->size;
    qp->size = 2 * oldSize;
    qp->array = realloc(qp->array, qp->size * sizeof(Command));
    assert (qp->array != NULL);
    for (i = 0; i < qp->back; i++) { /* eliminate split */
        qp->array[oldSize + i] = qp->array[i];
    }
    qp->back = qp->back + oldSize; /* update qp . back */
}

// Frees the memory of a queue
void freeQueue(CommandQueue q) {
    free(q.array);
}

// Enqueues a process to the queue
void enqueue(Command item, CommandQueue *qp) {
    qp->array[qp->back] = item;
    qp->back = (qp->back + 1) % qp->size;
    if (qp->back == qp->front) {
        doubleQueueSize(qp);
    }
}

// Dequeues a process from the queue
Command dequeue(CommandQueue *qp) {
    Command item;
    if (isEmptyQueue(*qp)) {
        queueEmptyError();
    }
    item = qp->array[qp->front];
    qp->front = (qp->front + 1) % qp->size;
    return item;
}