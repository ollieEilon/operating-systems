#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>

// Represents a command, this is useful in case you want to execute the command later
typedef struct {
    bool (*functionPtr)(char *name, char **commandArgs, int *status);
    char *name;
    char **commandArgs;
} Command;

// Represents a pipeline, this is useful in case you want to execute the pipeline later
typedef struct {
    bool (*functionPtr)(char *name, char **commandArgs, int *status);
    char *name;
    char **commandArgs;
} Pipeline;

Command newCommand(char *name, char **commandArgs);
Pipeline newPipeline(Command command);
bool executeThisCommand(Command command, int *status);
bool executeThisPipeline(Pipeline pipeline, int *status);

#endif