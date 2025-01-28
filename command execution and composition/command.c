#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"

// This file is responsible for handeling the commands and their execution

// Execute a command
bool executeCommand(char *name, char **commandArgs, int *status) {
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork() could not create a child process!\n");
    } else if (pid == 0) { // Only child process gets here
        // Execute program, leaves the world of C
        // Second argument is a list of string, where the first is the program name
        // This should take in as much as a command
        execvp(name, commandArgs);
        free(commandArgs);
        return false;
    }
    // Only parent process gets here
    wait(status); // Wait for the child process to finish 
    *status = WEXITSTATUS(*status);
    free(commandArgs);
    return true;
}

// Creates a new command to be executed later
Command newCommand(char *name, char **commandArgs) {
    Command this;
    this.name = name;
    this.commandArgs = commandArgs;
    this.functionPtr = executeCommand;
    return this;
}

// Creates a new pipeline to be executed later
Pipeline newPipeline(Command command) {
    Pipeline this;
    this.name = command.name;
    this.commandArgs = command.commandArgs;
    this.functionPtr = command.functionPtr;
    return this;
}

// Actually executes the command
bool executeThisCommand(Command command, int *status) {
    return (*command.functionPtr)(command.name, command.commandArgs, status);
}

// Actually executes the pipeline
bool executeThisPipeline(Pipeline pipeline, int *status) {
    return (*pipeline.functionPtr)(pipeline.name, pipeline.commandArgs, status);
}