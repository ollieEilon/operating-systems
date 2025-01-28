#ifndef BUILTINCOMMANDS_H
#define BUILTINCOMMANDS_H

#include "state.h"

// Builtin commands is exclusively for the implementation of the builtin commands

void bExit(char **args, State *state);
void bStatus(char **args, State *state);
void bEOF(char **args, State *state);
void bCd(char **args, State *state);

#ifdef EXT_PROMPT
// BONUS Lab 1! The colour command changes the colour of the prompt
void bSetColour(char **args, State *state);

// BONUS Lab 2! The pushd command pushes the current directory onto a stack
void bPushd(char **args, State *state);
void bPopd(char **args, State *state);
void bDirs(char **args, State *state);
#endif


#endif