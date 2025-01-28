#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>

#include "scanner.h"
#include "state.h"

#define INITIAL_ARG_LEN 10

bool interpretInputLine(List *lp, BuiltinMapper mapper, State *state);

#endif