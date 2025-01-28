#ifndef INC_2_PROCESSQUEUE_H
#define INC_2_PROCESSQUEUE_H

#include "process.h"

// A queue of processes that wait to be worked on by a processor
typedef struct {
    Process *array;
    int back;
    int front;
    int size;
} ProcessQueue;

ProcessQueue newQueue();
int isEmptyQueue(ProcessQueue q);
void queueEmptyError();
void doubleQueueSize(ProcessQueue *qp);
void enqueue(Process item, ProcessQueue *qp);
Process dequeue(ProcessQueue *qp);
void freeQueue(ProcessQueue q);

#endif //INC_2_PROCESSQUEUE_H