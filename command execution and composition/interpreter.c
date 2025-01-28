#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "builtin.h"
#include "interpreter.h"
#include "shell.h"
#include "command.h"

// This is where the fun happens. This file is responsible for interpreting the input and executing the commands.
// It assumes, the input is a valid command, as the parser has already checked for syntax errors.

// Gets the name of the current executable.
void getExecutableName(List *lp, char **name) {
    *name = (*lp)->t;
}

// Gets all the options to the current command (go until the end or the next operator)
void getOptions(List *lp, char ***commandArgs) {
    int argLen = INITIAL_ARG_LEN;
    *commandArgs = malloc(argLen * sizeof(char *));
    int argAmount = 1;

    while (*lp != NULL && !isOperator((*lp)->t)) {
        if (argAmount >= argLen) {
            argLen *= 2;
            *commandArgs = realloc(*commandArgs, argLen * sizeof(char *));
        }
        (*commandArgs)[argAmount - 1] = (*lp)->t;
        (*lp) = (*lp)->next;
        argAmount++;
    }
    (*commandArgs)[argAmount - 1] = NULL;
}


// Interprets a command but getting its name and options. Finds the command and puts it in *command.
void interpretCommand(List *lp, Command *command) {
    char *name;
    getExecutableName(lp, &name);

    char **commandArgs;
    getOptions(lp, &commandArgs);

    *command = newCommand(name, commandArgs);
    return;
}

// Interprets a pipeline, which for now is just a command
// todo: implement pipelines
void interpretPipeline(List *lp, Pipeline *pipeline) {
    Command command;
    interpretCommand(lp, &command);

    *pipeline = newPipeline(command);

    return;
}

// Interprets a filename, and puts it in *fileName
void interpretFileName(List *lp, char **fileName) {
    *fileName = (*lp)->t;
    *lp = (*lp)->next;
}

// Sets a specific file as the input file
void setAsInFile(char *inName) {
    int readFileId = open(inName, O_RDONLY, 0);
    dup2(readFileId, STDIN_FILENO);
    close(readFileId);
}

// Sets a specific file as the output file
void setAsOutFile(char *outName) {
    int writeFileId = creat(outName, 0644);
    dup2(writeFileId, STDOUT_FILENO);
    close(writeFileId);
}

// Interprets redirections, which for now is just input and output redirection
// This is actually going a bit in advance, but I we decided to include it anyway
void interpretRedirections(List *lp) {
    if (isEmpty(*lp)) {
        return;
    }

    char *inName;
    char *outName;

    if (acceptToken(lp, "<")) {
        interpretFileName(lp, &inName);

        setAsInFile(inName);
        
        if (acceptToken(lp, ">")) {
            interpretFileName(lp, &outName);

            setAsOutFile(outName);
        }

    } else if (acceptToken(lp, ">")) {
        interpretFileName(lp, &outName);

        setAsOutFile(outName);

        if (acceptToken(lp, "<")) {
            interpretFileName(lp, &inName);
            setAsInFile(inName);
        }
    }
}

// Interprets a builtin command and sets it in *builtin. Returns true if the builtin was found, false otherwise.
// The return value can be used to give error messages and deciding between a builtin and a normal executable.
bool findBuiltIn(List *lp, BuiltinMapper builtinMapper, Builtin *builtin) {
    return getBuiltin(builtinMapper, (*lp)->t, builtin);
}


// Interprets a chain, which is a pipeline or a builtin command
bool interpretChain(List *lp, BuiltinMapper mapper, State *state) {
    // Builtin
    Builtin builtin;
    if (findBuiltIn(lp, mapper, &builtin)) {
        char **commandArgs;
        getOptions(lp, &commandArgs);

        // execute builtin
        (builtin.functionPtr)(commandArgs, state);

        free(commandArgs);
        return true;
    }

    // Pipeline
    Pipeline pipeline;
    interpretPipeline(lp, &pipeline);
    // First interpret the redirections
    interpretRedirections(lp);
    // Then execute the pipeline
    return executeThisPipeline(pipeline, &(state->lastExitCode));
}

// Interprets the operator and the rest of the inputline
// Uses mutual recursion O.o
bool interpretOperatorAndInputLine(List *lp, BuiltinMapper mapper, State *state) {
    if (acceptToken(lp, "&&")) {
        if (state->lastExitCode == 0) {
            return interpretInputLine(lp, mapper, state);
        }
        // get rid of the chain, as it is not going to be executed
        parseChain(lp);
        return interpretOperatorAndInputLine(lp, mapper, state);

    } else if (acceptToken(lp, "||")) {
        if (state->lastExitCode != 0) {
            return interpretInputLine(lp, mapper, state);
        }
        // get rid of the chain, as it is not going to be executed
        parseChain(lp);
        return interpretOperatorAndInputLine(lp, mapper, state);
    } else if (acceptToken(lp, ";")) {
        return interpretInputLine(lp, mapper, state);
        // TODO: implement &
    // } else if(acceptToken(lp, "&")) {
    //     return interpretInputLine(lp, builtinMapper, state);
    }
    return true;
}

// Interprets a full input line, which is a chain followed by an operator and another input line
bool interpretInputLine(List *lp, BuiltinMapper mapper, State *state) {
    if (isEmpty(*lp)) {
        return true;
    }

    return interpretChain(lp, mapper, state) && interpretOperatorAndInputLine(lp, mapper, state);
}