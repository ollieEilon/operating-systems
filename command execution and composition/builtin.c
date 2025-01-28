#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "builtin.h"
#include "colours.h"
#include "state.h"

// This file is responsible for handeling the builtin commands

// This is a global variable, but it's not a big deal because it's just a list of the names of the builtins
char *builtIns[] = {
    "exit",
    "status",
    "\x1A", // EOF
    #ifdef EXT_PROMPT
    "cd",
    #endif
    #ifdef EXT_PROMPT
    "colour",
    #endif
    NULL
};

// The exit command 
void bExit(char **args, State *state) {
    state->isRunning = false;
    state->lastExitCode = 0;
}

// Tje status command, it does not alter the status
void bStatus(char **args, State *state) {
    printf("The most recent exit code is: %d\n", state->lastExitCode);
}

// The EOF command, it does the same as exit
void bEOF(char **args, State *state) {
    bExit(args, state);
}

// BONUS! The cd command changes the current working directory
#ifdef EXT_PROMPT
void bCd(char **args, State *state) {
    if (args[1] != NULL) {
        if (chdir(args[1]) == 0) {
            state->lastExitCode = 0;
        } else {
            state->lastExitCode = 1;
            printf("Directory \"%s\" not found\n", args[1]);
        }
    }
}
#endif

// BONUS! The colour command changes the colour of the prompt
#ifdef EXT_PROMPT
void bSetColour(char **args, State *state) {
    if (args[1] == NULL) {
        state->lastExitCode = 1;
        printf("Expected argument to \"colour\"\n");
    } else {
        char *colourCode = getColourCode(state->colourMap, args[1]);
        if (colourCode != NULL) {
            printf("%s", colourCode);
            state->colourCode = colourCode;
        } else {
            state->lastExitCode = 1;
            printf("Invalid colour code\n");
        }
    }
}
#endif

// Constructs a new BuiltinMapping
BuiltinMapping newBuiltinMapping(const char *name, void (*functionPtr)(char **, State*)) {
    BuiltinMapping this;
    this.name = name;
    this.builtin.functionPtr = functionPtr;
    return this;
}

// Constructs a new BuiltinMapper
BuiltinMapper newBuiltinMapper() {
    BuiltinMapper this;

    this.count = 0;
    while (builtIns[this.count] != NULL) {
        this.count++;
    }

    BuiltinMapping *map = malloc(this.count * sizeof(BuiltinMapping));
    map[0] = newBuiltinMapping(builtIns[0], bExit);
    map[1] = newBuiltinMapping(builtIns[1], bStatus);
    map[2] = newBuiltinMapping(builtIns[2], bEOF);
    #ifdef EXT_PROMPT
    map[3] = newBuiltinMapping(builtIns[3], bCd);
    #endif
    #ifdef EXT_PROMPT
    map[4] = newBuiltinMapping(builtIns[4], bSetColour);
    #endif

    this.map = map;
    return this;
}

// Frees the memory of a BuiltinMapper
void freeBuiltinMapper(BuiltinMapper this) {
    free(this.map);
}

// Gets a builtin from a BuiltinMapper
bool getBuiltin(BuiltinMapper this, char *name, Builtin *response) {
    for (int i = 0; i < this.count; i++) {
        if (strcmp(name, this.map[i].name) == 0) {
            response->functionPtr = this.map[i].builtin.functionPtr;
            return true;
        }
    }
    return false;
}
