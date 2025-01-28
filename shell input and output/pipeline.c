#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "pipeline.h"
#include "state.h"

// Represents a pipe that connects two commands
// Is a struct, to make addressing in an array more readable
typedef struct {
    int fds[2];
} Pipe;

// Constructor for a pipeline
Pipeline newPipeline() {
    Pipeline this;
    this.queue = newQueue();
    return this;
}

// Enqueues a command to the pipeline
void enqueueCommand(Pipeline *this, Command command) {
    enqueue(command, &this->queue);
}

// Closes all pipes in an array of pipes
void closeAllPipes(Pipe *pipes, int amount) {
    for (int i = 0; i < amount; i++) {
        close(pipes[i].fds[0]);
        close(pipes[i].fds[1]);
    }
}

// Replaces stdout with the write end of a pipe
void setOutputPipe(Pipe pipe) {
    // replacing stdout with pipe write 
    dup2(pipe.fds[1], STDOUT_FILENO);
}

// Replaces stdin with the read end of a pipe
void setInputPipe(Pipe pipe) {
    // replacing stdin with pipe read
    dup2(pipe.fds[0], STDIN_FILENO);
}


bool executePipedCommand(Command command, State *state, Pipe *pipes, int pipeAmount) {
    closeAllPipes(pipes, pipeAmount);

    executeCommand(command, state);

    // Free this commands memory
    freeCommand(command);
    free(pipes);
    return false;
}

bool executeUnitPipeline(Command command, State *state) {
    if (fork() == 0) {
        executeCommand(command, state);
        freeCommand(command);
        return false;
    }
    freeCommand(command);
    // wait for child to finish
    wait(&state->lastExitCode);
    state->lastExitCode = WEXITSTATUS(state->lastExitCode);
    return true;
}


// Executes the pipeline
bool executePipeline(Pipeline *this, State *state) {
    CommandQueue *queue = &this->queue;
    int commandAmount = getQueueSize(*queue);

    // If there is nothing to pipe:
    if (commandAmount == 1) {
        return executeUnitPipeline(dequeue(queue), state);
    }

    // Now set up pipes
    int pipeAmount = commandAmount - 1;
    Pipe *pipes = malloc(pipeAmount * sizeof(Pipe));
    
    // Create pipes
    for (int i = 0; i < pipeAmount; i++) {
        if (pipe(pipes[i].fds) == -1) {
            perror("Pipe failed\n");
            exit(1);
        }
    }

    // Process first command
    Command currentCommand = dequeue(queue);
    if (fork() == 0) {
        setOutputPipe(pipes[0]);
        if (!executePipedCommand(currentCommand, state, pipes, pipeAmount)) {
            return false;
        }
    }
    freeCommand(currentCommand);

    // Process middle commands
    for (int i = 1; i < pipeAmount; i++) {
        currentCommand = dequeue(queue);
        if (fork() == 0) {
            setOutputPipe(pipes[i]);
            setInputPipe(pipes[i - 1]);
            if (!executePipedCommand(currentCommand, state, pipes, pipeAmount)) {
                return false;
            }
        }
        freeCommand(currentCommand);
    }

    // Process last command
    currentCommand = dequeue(queue);
    if (fork() == 0) {
        setInputPipe(pipes[pipeAmount - 1]);
        if (!executePipedCommand(currentCommand, state, pipes, pipeAmount)) {
            return false;
        }
    }
    freeCommand(currentCommand);


    closeAllPipes(pipes, commandAmount - 1);
    // wait for all children to finish
    for (int i = 0; i < commandAmount; i++) {
        wait(&state->lastExitCode); // Wait for the child process to finish 
        state->lastExitCode = WEXITSTATUS(state->lastExitCode);
    }
    free(pipes);
    return true;
}

// Frees the entire pipeline frees all internal commands in case there are still some inside
void freePipeline(Pipeline this) {
    if (!isEmptyQueue(this.queue)) {
        while (!isEmptyQueue(this.queue)) {
            freeCommand(dequeue(&this.queue));
        }
    }
    freeQueue(this.queue);
}