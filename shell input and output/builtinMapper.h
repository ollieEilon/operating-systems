#ifndef BUILTINMAPPER_H
#define BUILTINMAPPER_H

#include "state.h"

// A function pointer to a builtin command
typedef void (*BuiltinFunction)(char **, State *);


// A builtin command with arguments
typedef struct {
    char *name;
    char **commandArgs;
    BuiltinFunction function;
} BuiltinCommand;

// A mapping from a name to a builtin command
typedef struct {
    const char *name;
    BuiltinFunction function;
} BuiltinMapping;

// A list of mappings from names to builtin commands
struct BuiltinMapper{
    BuiltinMapping *map;
    int count;
};

// global variable O.o, but it's not a big deal because it's just a list of the names of the builtins
// It is there as the list of available builtins is meant to be public (more than the builtins themselves)
extern char *builtIns[];

BuiltinMapper newBuiltinMapper();
void freeBuiltinMapper(BuiltinMapper this);
bool getBuiltin(BuiltinMapper this, char *name, BuiltinFunction *response);
BuiltinCommand newBuiltinCommand(char *name, char **commandArgs, BuiltinFunction functionPtr);
void executeBuiltin(BuiltinCommand builtin, State *state);
void freeBuiltinCommand(BuiltinCommand *this);

#endif