#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "builtinMapper.h"
#include "builtinCommands.h"
#include "state.h"
#include "directories.h"
#ifdef EXT_PROMPT
#include "colors.h"
#include "directoryStack.h"
#endif

// This file is responsible for handling the builtin commands by providing a mapping

// This is a global variable, but it's not a big deal because it's just a list of the names of the builtins
char *builtIns[] = {
    "exit",
    "status",
    "\x1A", // EOF
    "cd",
    #ifdef EXT_PROMPT
    "colour",
    // BONUS Lab 2! The new builtin commands
    "pushd",
    "popd",
    "dirs",
    #endif
    NULL
};

// Constructs a new BuiltinMapping
BuiltinMapping newBuiltinMapping(const char *name, BuiltinFunction function) {
    BuiltinMapping this;
    this.name = name;
    this.function = function;
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
    map[3] = newBuiltinMapping(builtIns[3], bCd);
    #ifdef EXT_PROMPT
    map[4] = newBuiltinMapping(builtIns[4], bSetColour);
    map[5] = newBuiltinMapping(builtIns[5], bPushd);
    map[6] = newBuiltinMapping(builtIns[6], bPopd);
    map[7] = newBuiltinMapping(builtIns[7], bDirs);
    #endif

    this.map = map;
    return this;
}

// Frees the memory of a BuiltinMapper
void freeBuiltinMapper(BuiltinMapper this) {
    free(this.map);
}

// Gets a builtin from a BuiltinMapper
bool getBuiltin(BuiltinMapper this, char *name, BuiltinFunction *response) {
    for (int i = 0; i < this.count; i++) {
        if (strcmp(name, this.map[i].name) == 0) {
            *response = this.map[i].function;
            return true;
        }
    }
    return false;
}

// Constructs a new BuiltinCommand, symmetrical to a normal command
BuiltinCommand newBuiltinCommand(char *name, char **commandArgs, BuiltinFunction function) {
    BuiltinCommand this;
    this.name = name;
    this.commandArgs = commandArgs;
    this.function = function;
    return this;
}

// Executes a builtin command
void executeBuiltin(BuiltinCommand builtin, State *state) {
    (*builtin.function)(builtin.commandArgs, state);
}

// Frees the memory of a BuiltinCommand
void freeBuiltinCommand(BuiltinCommand *this) {
    free(this->commandArgs);
}