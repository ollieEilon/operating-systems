#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "process.h"

// Size of the initial array to store processes
#define INITIAL_SIZE 10

Processes newProcesses() {
    Processes p;
    p.size = INITIAL_SIZE;
    p.array = malloc(p.size * sizeof(Process));
    p.at = 0;
    return p;
}

// Scans a process from the standard input
// Returns flase if it did not scan a process (EOF or -2)
bool scanProcess(Process *this) {
//    -2 to manually end
    if (scanf("%d", &(this->arrivalTime)) == EOF || this->arrivalTime == -2) {
        return false;
    }
    this->times = scanList();
    return this;
}

// Scans a list of processes from the standard input
Processes scanProcesses() {
    Processes p = newProcesses();
    int i = 0;
    while (scanProcess(p.array + i)) {
        i++;
        if (i == p.size) {
            p.size *= 2;
            p.array = realloc(p.array, p.size * sizeof(Process));
        }
    }
    p.size = i;
    return p;
}

// Peeks at the next process in the list of processes
Process peekNextProcess(Processes this) {
    assert(this.at < this.size);
    return this.array[this.at];
}

// Polls the next process in the list of processes
Process pollNextProcess(Processes *this) {
    assert(this->at < this->size);
    return this->array[this->at++];
}

// Frees the memory of a process
void freeProcess(Process p) {
    freeList(p.times);
}

// Frees the memory of a list of processes
void freeProcesses(Processes p) {
    for (int i = 0; i < p.size; i++) {
        freeProcess(p.array[i]);
    }
    free(p.array);
}

// Gets the amount of time the first job in the process will take
int getFirstStepDuration(Process this) {
    return getFirst(this.times);
}

// Move the process to the next job.
// If the process is finished, return true, else return false
bool finishStep(Process *this) {
    this->times = this->times->next;
    if (isFinished(*this)) {
        return true;
    }
    return false;
}

// Process the first job of the process for a certain amount of time
// Should only be used if the amount of time does not exceed the time the job will take
void processFor(Process *this, int howLong) {
    this->times->value -= howLong;
    assert(this->times->value > 0);
}

// Checks whether the process is finished
bool isFinished(Process this) {
    return isEmpty(this.times);
}