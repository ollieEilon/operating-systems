#include <stdio.h>
#include <limits.h>

#include "process.h"
#include "processor.h"

void enqueueFreshProcesses(Processes *freshProcesses, Processor *cpu, long int time) {
    Process in;
    while (freshProcesses->at < freshProcesses->size && peekNextProcess(*freshProcesses).arrivalTime <= time) {
        in = pollNextProcess(freshProcesses);
        enqueueProcess(cpu, in);
    }
}

// This program takes in a number of processes and schedules them using a fist come first serve algorithm
// It then prints out the average turnaround time of the processes

// Here it all begins
int main() {
    // They are in an arry and not scanned in as we go, as to accomodate for the option of them being out of order
    Processes givenProcesses = scanProcesses();

    // Create two processors, one for the CPU and one for the IO
    Processor cpu = newProcessor();
    Processor io = newProcessor();

    // For all the processes, subtract the arrival time to the total turnaround time
    // as totalTurnaroundTime = sum(turnaroundTime - arrivalTime) = sum(turnaroundTime) - sum(arrivalTime)
    long int totalTurnaroundTime = 0;
    for (int i = 0; i < givenProcesses.size; i++) {
        totalTurnaroundTime -= givenProcesses.array[i].arrivalTime;
    }

    // longs for overflow protection
    long int time = 0;

    // While there are still processes to work on
    while(givenProcesses.at != givenProcesses.size || hasSomethingToWorkOn(cpu) || hasSomethingToWorkOn(io)) {

        // In the amount of time that has passed, enqueue all the processes that have arrived
        enqueueFreshProcesses(&givenProcesses, &cpu, time);

        // Call methods, to dequeue the first proces and get it into working position
        // also gets the time it will take to finish the first step
        int cpuTime = getReadyToWork(&cpu);
        int ioNeeded = getReadyToWork(&io);

        // Get the minimum of the two times
        int workTime = cpuTime < ioNeeded ? cpuTime : ioNeeded;

        // If there is no process to work on, just increment the time
        if (workTime == INT_MAX) {
            time += 1;
            continue;
        }

        // Increment the time by the amount of time it will take to finish the first step
        time += workTime;

        // Now that the time advanced, we can still enqueue the processes that have arrived,
        // as they have priority over the ones that finish processing
        enqueueFreshProcesses(&givenProcesses, &cpu, time);

        // Work on the process for the amount of time we found out is the biggest to move forward
        // If the process is finished, add the current time to the total turnaround time (totalTurnaroundTime = sum(turnaroundTime) - sum(arrivalTime))
        if (work(&cpu, workTime, &io)) {
            totalTurnaroundTime += time;
        }
        if (work(&io, workTime, &cpu)) {
            totalTurnaroundTime += time;
        }
    }
    // Find average
    long double averageTurnaroundTime = ((long double) totalTurnaroundTime) / givenProcesses.size;

    // Print the average turnaround time
    printf("%.0Lf\n", averageTurnaroundTime);

    freeProcessor(cpu);
    freeProcessor(io);
    freeProcesses(givenProcesses);
}