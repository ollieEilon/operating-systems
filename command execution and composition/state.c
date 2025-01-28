#include <stdbool.h>

#include "state.h"
#include "colours.h"

// Manages the state of the shell

// Create a new state, used only once
State newDefaultState() {
    State this;
    this.lastExitCode = 0;
    this.isRunning = true;
    #ifdef EXT_PROMPT
    this.colourMap = getColourMap();
    this.colourCode = DEFAULT_COLOUR;
    #endif
    return this;
}

// Free the state
void freeState(State state) {
    #ifdef EXT_PROMPT
    freeColourMap(state.colourMap);
    #endif
}