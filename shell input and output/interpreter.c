#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "builtinMapper.h"
#include "interpreter.h"
#include "parser.h"
#include "commandQueue.h"
#include "command.h"
#include "pipeline.h"
#include "redirections.h"

// This is where the fun happens. This file is responsible for interpreting the input and executing the commands.
// It assumes, the input is a valid command, as the parser has already checked for syntax errors.

// Gets the name of the current executable.
char *getExecutableName(List *lp) {
    return (*lp)->t;
}

// Gets all the options to the current command (go until the end or the next operator)
// Allocs memory
char **getOptions(List *lp) {
    int argLen = INITIAL_ARG_LEN;
    char **commandArgs = malloc(argLen * sizeof(char *));
    int argAmount = 1;
    // In case an operator sneaks through, it will still "eat up" one operator
    bool isFirst = true;
    while (isFirst || (*lp != NULL && !isOperator((*lp)->t))) {
        if (argAmount >= argLen) {
            argLen *= 2;
            commandArgs = realloc(commandArgs, argLen * sizeof(char *));
        }
        commandArgs[argAmount - 1] = (*lp)->t;
        (*lp) = (*lp)->next;
        argAmount++;
        isFirst = false;
    }
    commandArgs[argAmount - 1] = NULL;
    return commandArgs;
}


// Interprets a command but getting its name and options. Finds the command and puts it in *command.
Command interpretCommand(List *lp) {
    char *name = getExecutableName(lp);

    char **commandArgs = getOptions(lp);

    return newCommand(name, commandArgs);
}

// Interprets a pipeline, actually does it now
Pipeline interpretPipeline(List *lp) {
    Pipeline pipeline = newPipeline();
    do {
        Command command = interpretCommand(lp);
        enqueueCommand(&pipeline, command);
    } while (acceptToken(lp, "|"));

    return pipeline;
}

// Interprets a filename, and puts it in *fileName
char *interpretFileName(List *lp) {
    char *fileName = (*lp)->t;
    *lp = (*lp)->next;
    return fileName;
}


// Interprets redirections, which for now is just input and output redirection
bool interpretRedirections(List *lp, State state) {
    if (isEmpty(*lp)) {
        return true;
    }
    char *inName = NULL;
    char *outName = NULL;
    bool alsoErrorIO = false;

    // execute exactly twice to allow for < > and > <, and checks to disallow < < and > >
    for (int i = 0; i < 2; i++) {
        if (inName == NULL && acceptToken(lp, "<")) {
            inName = interpretFileName(lp);
        }
        if (outName == NULL) {
            if (acceptToken(lp, ">")) {
                outName = interpretFileName(lp);
            #ifdef EXT_PROMPT
            // BONUS Lab 2! Allow for error redirection
            } else if (acceptToken(lp, "n>")) {
                outName = interpretFileName(lp);
                alsoErrorIO = true;
            #endif
            }

        }
    }

    // If the input and output files are the same, print an error and return false
    if (inName != NULL && outName != NULL && strcmp(inName, outName) == 0) {
        printError(state, "Error: input and output files cannot be equal!");
        return false;
    }

    if (inName != NULL) {
        setAsInFile(inName);
    }
    if (outName != NULL) {
        if (alsoErrorIO) {
            setAsAnyOutFile(outName);
        } else {
            setAsStdOutFile(outName);
        }
    }
    return true;
}

// Interprets a builtin command and sets it in *builtin. Returns true if the builtin was found, false otherwise.
// The return value can be used to give error messages and deciding between a builtin and a normal executable.
bool findBuiltIn(List *lp, BuiltinMapper builtinMapper, BuiltinFunction *builtin) {
    return getBuiltin(builtinMapper, (*lp)->t, builtin);
}


// Interprets a chain, which is a pipeline or a builtin command
bool interpretChain(List *lp, State *state) {
    // BuiltinFunction
    BuiltinFunction builtin;
    if (findBuiltIn(lp, *(state->builtinMapperP), &builtin)) {
        char **commandArgs = getOptions(lp);
        // BuiltinCommand
        BuiltinCommand builtinCommand = newBuiltinCommand(commandArgs[0], commandArgs, builtin);

        executeBuiltin(builtinCommand, state);

        // execute builtin
        freeBuiltinCommand(&builtinCommand);
        return true;
    }

    // Pipeline
    Pipeline pipeline = interpretPipeline(lp);


    // Save the current IO
    int originalStdin, originalStdout, originalStderr;
    getCurrentIO(&originalStdin, &originalStdout, &originalStderr);


    // First interpret the redirections
    bool redirectionSuccess = interpretRedirections(lp, *state);

    bool pipelineSuccess = true;
    if (redirectionSuccess) {
        pipelineSuccess = executePipeline(&pipeline, state);
    }

    // Reset the IO to defaults 
    resetIO(originalStdin, originalStdout, originalStderr);
    freePipeline(pipeline);
    return pipelineSuccess;
}

// Interprets the operator and the rest of the inputline
// Uses mutual recursion O.o
bool interpretOperatorAndInputLine(List *lp, State *state) {
    if (acceptToken(lp, "&&")) {
        if (state->lastExitCode == 0) {
            return interpretInputLine(lp, state);
        }
        // get rid of the chain, as it is not going to be executed
        parseChain(lp);
        return interpretOperatorAndInputLine(lp, state);

    } else if (acceptToken(lp, "||")) {
        if (state->lastExitCode != 0) {
            return interpretInputLine(lp, state);
        }
        // get rid of the chain, as it is not going to be executed
        parseChain(lp);
        return interpretOperatorAndInputLine(lp, state);
    } else if (acceptToken(lp, ";")) {
        return interpretInputLine(lp, state);
        // TODO: implement &
    // } else if(acceptToken(lp, "&")) {
    //     return interpretInputLine(lp, state);
    }
    return true;
}

// Interprets a full input line, which is a chain followed by an operator and another input line
// The return value is true, if its the parent that returns, false if a child returns
bool interpretInputLine(List *lp, State *state) {
    if (isEmpty(*lp)) {
        return true;
    }

    return interpretChain(lp, state) && interpretOperatorAndInputLine(lp, state);
}