#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include "state.h"

// Some handy (personal) abstractions for dealing with directories
bool changeToDirectory(char *directory);
bool doesDirectoryExist(char *directory);
char *getCurrentWorkingDirectory(State *state);

#endif