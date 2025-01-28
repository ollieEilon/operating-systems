#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "directories.h"

#define MAX_PATH 1024

// Change to the directory
// Returns true if successful, false otherwise
bool changeToDirectory(char *directory) {
    return chdir(directory) == 0;
}

// Check if the directory exists
struct stat info;
bool doesDirectoryExist(char *directory) {
    if (stat(directory, &info) != 0) {
        return false;
    }
    return true;
}

// Get the current working directory, allocates memory
// Used in BONUS Lab 2!
char *getCurrentWorkingDirectory(State *state) {
    char *cwd = malloc(MAX_PATH * sizeof(char));
    if (getcwd(cwd, 1024) == NULL) {
        state->lastExitCode = 2;
        printError(*state, "Error: could not get current working directory!");
        return NULL;
    }
    return cwd;
}