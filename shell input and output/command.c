#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "command.h"

// Constructor for a command
Command newCommand(char *name, char **commandArgs) {
    Command this;
    this.name = name;
    this.commandArgs = commandArgs;
    return this;
}

// Execute the command, right now it just calls execvp
void executeCommand(Command this, State *state) {
    execvp(this.name, this.commandArgs);
}

// Free the command
void freeCommand(Command this) {
    free(this.commandArgs);
}