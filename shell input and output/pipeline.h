#ifndef PIPELINE_H
#define PIPELINE_H

#include "commandQueue.h"
#include "state.h"

// Represents a pipeline of commands
// Is actually just a queue
typedef struct {
    CommandQueue queue;
} Pipeline;

Pipeline newPipeline();
void enqueueCommand(Pipeline *this, Command command);
bool executePipeline(Pipeline *this, State *state);
void freePipeline(Pipeline this);



#endif