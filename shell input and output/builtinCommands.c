#include <stdio.h>
#include <stdlib.h>

#include "builtinCommands.h"
#include "state.h"
#include "directories.h"
#ifdef EXT_PROMPT
#include "directoryStack.h"
#endif


// The exit command 
void bExit(char **args, State *state) {
    state->isRunning = false;
    state->lastExitCode = 0;
}

// The status command, it does not alter the status
void bStatus(char **args, State *state) {
    printf("The most recent exit code is: %d\n", state->lastExitCode);
}

// The EOF command, it does the same as exit
void bEOF(char **args, State *state) {
    bExit(args, state);
}

// No longer bonus :( The cd command changes the current working directory
void bCd(char **args, State *state) {
    if (args[1] == NULL) {
        printError(*state, "Error: cd requires folder to navigate to!");
        state->lastExitCode = 2;
        return;
    }
    if (!changeToDirectory(args[1])) {
        printError(*state, "Error: cd directory not found!");
        state->lastExitCode = 2;
        return;
    }
    state->lastExitCode = 0;
}


#ifdef EXT_PROMPT
// BONUS Lab 1! The colour command changes the colour of the prompt
void bSetColour(char **args, State *state) {
    if (args[1] == NULL) {
        printf("Expected argument to \"colour\"\n");
        state->lastExitCode = 2;
        return;
    }

    char *colourCode = getColourCode(state->colourMap, args[1]);
    if (colourCode == NULL) {
        printf("Invalid colour code\n");
        state->lastExitCode = 2;
        return;
    }
    printf("%s", colourCode);
    state->colourCode = colourCode;
    state->lastExitCode = 0;
}

// BONUS Lab 2! The pushd command pushes the current directory onto a stack
void bPushd(char **args, State *state) {
    DirectoryStack *stackP = state->directoryStackP;
    if (args[1] == NULL) {
        pushdNoArgs(stackP, state);
        return;
    }
    pushdWithArgs(stackP, args[1], state);
}
    
void bPopd(char **args, State *state) {
    popd(state->directoryStackP, state);
}

void bDirs(char **args, State *state) {
    dirs(*(state->directoryStackP), state);
}
#endif