#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include <stdbool.h>

#include "command.h"

// A queue of commands that wait to be worked on by a processor
typedef struct {
    Command *array;
    int back;
    int front;
    int size;
} CommandQueue;

CommandQueue newQueue();
bool isEmptyQueue(CommandQueue q);
int getQueueSize(CommandQueue q);
void enqueue(Command item, CommandQueue *qp);
Command dequeue(CommandQueue *qp);
void freeQueue(CommandQueue queue);

#endif