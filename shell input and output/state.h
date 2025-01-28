#ifndef STATE_H
#define STATE_H

#include <stdbool.h>
#ifdef EXT_PROMPT
#include "colors.h"
#endif

// Do some forward declaration to avoid circular dependencies
typedef struct BuiltinMapper BuiltinMapper;
#ifdef EXT_PROMPT
typedef struct DirectoryStack DirectoryStack;
#endif

// Holds all needed information about the state of the shell
// Also holds useful resources, like look up tables
// It was meant to hold BuiltinMapper, but that would have lead to a circular dependency
typedef struct {
    int lastExitCode;
    bool isRunning;
    // A state is not responsible for freeing the BuiltinMapper
    struct BuiltinMapper *builtinMapperP;
    #ifdef EXT_PROMPT
    ColourMap colourMap;
    char *colourCode;
    DirectoryStack *directoryStackP;
    #endif
} State;

State newDefaultState();
void addBuiltinMapper(State *state, BuiltinMapper *mapper);
#ifdef EXT_PROMPT
void addDirectoryStack(State *state, DirectoryStack *stack);
#endif
void printError(State state, char *message);
void freeState(State state);

#endif