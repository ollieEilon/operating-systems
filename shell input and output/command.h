#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

#include "state.h"

// A command with arguments
typedef struct {
    char *name;
    char **commandArgs;
} Command;

Command newCommand(char *name, char **commandArgs);
void executeCommand(Command this, State *state);
void freeCommand(Command this);

#endif