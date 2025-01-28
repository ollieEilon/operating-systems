#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H


// Has code responsible for redirecting input and output
void setAsInFile(char *inName);
void setAsStdOutFile(char *outName);
void setAsAnyOutFile(char *outName);
void resetIO(int originalStdin, int originalStdout, int originalStderr);
void setAsErrFile(char *errName);
void getCurrentIO(int *originalStdin, int *originalStdout, int *originalStderr);

#endif // REDIRECTIONS_H