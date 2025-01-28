#ifndef STATE_H
#define STATE_H

#include "colours.h"

// Holds all needed information about the state of the shell
// Also holds useful resources, like look up tables
// It was meant to hold BuiltinMapper, but that would have lead to a circular dependency
typedef struct {
    int lastExitCode;
    bool isRunning;
    #ifdef EXT_PROMPT
    ColourMap colourMap;
    char *colourCode;
    #endif
} State;

State newDefaultState();
void freeState(State state);

#endif