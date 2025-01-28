#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// Sets a specific file as the input file
void setAsInFile(char *inName) {
    int readFileId = open(inName, O_RDONLY, 0);
    dup2(readFileId, STDIN_FILENO);
    close(readFileId);
}

// Sets a specific file as a specific file descriptor
void setAsOutFile(char *fileName, int fileDescriptor) {
}

// Sets a specific file as the standard output file
void setAsStdOutFile(char *outName) {
    int writeFileId = creat(outName, 0644);
    dup2(writeFileId, STDOUT_FILENO);
    close(writeFileId);
}


// Sets a specific file as the error file
void setAsAnyOutFile(char *outName) {
    int writeFileId = creat(outName, 0644);
    dup2(writeFileId, STDOUT_FILENO);
    dup2(writeFileId, STDERR_FILENO);
    close(writeFileId);
}


// Resets the I/O to the original state, including the error I/O
void resetIO(int originalStdin, int originalStdout, int originalStderr) {
    dup2(originalStdin, STDIN_FILENO);
    dup2(originalStdout, STDOUT_FILENO);
    dup2(originalStderr, STDERR_FILENO);
}



// Gets the current I/O, as well as the error I/O
void getCurrentIO(int *originalStdin, int *originalStdout, int *originalStderr) {
    *originalStdin = dup(STDIN_FILENO);
    *originalStdout = dup(STDOUT_FILENO);
    *originalStderr = dup(STDERR_FILENO);
}