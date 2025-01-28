#ifndef BUILTIN_H
#define BUILTIN_H

#include "state.h"

// A builtin command
typedef struct {
    void (*functionPtr)(char **, State *);
} Builtin;

// A mapping from a name to a builtin command
typedef struct {
    const char *name;
    Builtin builtin;
} BuiltinMapping;

// A list of mappings from names to builtin commands
typedef struct {
    BuiltinMapping *map;
    int count;
} BuiltinMapper;

// global variable O.o, but it's not a big deal because it's just a list of the names of the builtins
extern char *builtIns[];

BuiltinMapper newBuiltinMapper();
void freeBuiltinMapper(BuiltinMapper this);
bool getBuiltin(BuiltinMapper this, char *name, Builtin *response);

#endif