#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "processor.h"
#include "processQueue.h"

// Instantiates a new processor
Processor newProcessor() {
    Processor this;
    this.queue = newQueue();
    this.isWorking = false;
    return this;
}

// Frees the memory of a processor
void freeProcessor(Processor p) {
    freeQueue(p.queue);
}

// Checks whether the processor has something to work on
bool hasSomethingToWorkOn(Processor this) {
    return this.isWorking || !isEmptyQueue(this.queue);
}

// Enqueues a process to the processor
void enqueueProcess(Processor *this, Process process) {
    enqueue(process, &this->queue);
}

// Gets the time it will take to finish the first step of the first process
// Also moves the process to the workingOn field
int getReadyToWork(Processor *this) {
    if (this->isWorking) {
        return getFirstStepDuration(this->workingOn);
    }
    if (isEmptyQueue(this->queue)) {
        // If there is nothing to work on, return INT_MAX
        return INT_MAX;
    }
    this->workingOn = dequeue(&this->queue);
    this->isWorking = true;

    return getFirstStepDuration(this->workingOn);
}

// Processes the first step of the workingOn process for a certain amount of time
// Returns whether the entire process has finished
bool work(Processor *this, int howLong, Processor *other) {
    bool response = false;
    if (this->isWorking) {
        if (getFirstStepDuration(this->workingOn) <= howLong) {
            
            // If we finished the process, the response may be true
            response = finishStep(&(this->workingOn));
            if (!isFinished(this->workingOn)) {
                enqueueProcess(other, this->workingOn);
            }
            this->isWorking = false;
        } else {
            processFor(&this->workingOn, howLong);
        }
    }
    return response;
}