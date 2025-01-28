#ifndef INC_2_PROCESSOR_H
#define INC_2_PROCESSOR_H

#include "processQueue.h"

// A processor is a queue of processes, and a process that is currently being worked on
typedef struct {
    ProcessQueue queue;
    Process workingOn;
    bool isWorking;
} Processor;

Processor newProcessor();
void freeProcessor(Processor p);
bool hasSomethingToWorkOn(Processor this);
void enqueueProcess(Processor *this, Process process);
int getReadyToWork(Processor *this);
bool work(Processor *this, int howLong, Processor *other);

#endif //INC_2_PROCESSOR_H