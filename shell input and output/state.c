#include <stdio.h>
#include <stdbool.h>


#include "state.h"

#ifdef EXT_PROMPT
#include "colors.h"
#include "directoryStack.h"
#endif

// Manages the state of the shell

// Create a new state, used only once
State newDefaultState() {
    State this;
    this.lastExitCode = 0;
    this.isRunning = true;
    this.builtinMapperP = NULL;
    #ifdef EXT_PROMPT
    this.colourMap = getColorMap();
    this.colourCode = DEFAULT_COLOR;
    this.directoryStackP = NULL;
    #endif
    return this;
}

// Add a builtin mapper to the state
void addBuiltinMapper(State *state, BuiltinMapper *mapper) {
    state->builtinMapperP = mapper;
}

#ifdef EXT_PROMPT
// Add a directory stack to the state
void addDirectoryStack(State *state, DirectoryStack *stack) {
    state->directoryStackP = stack;
}
#endif

// Print an error message (BONUS Lab 1! in red)
void printError(State state, char *message) {
    #ifdef EXT_PROMPT
    printf("\033[0;31m"); // Red
    #endif

    printf("%s\n", message);

    #ifdef EXT_PROMPT
    printf("%s", state.colourCode); // Reset
    #endif
}

// Free the state
void freeState(State state) {
    #ifdef EXT_PROMPT
    freeColourMap(state.colourMap);
    #endif
}