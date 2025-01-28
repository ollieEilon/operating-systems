#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

#include "scanner.h"
#include "parser.h"
#include "state.h"
#include "builtinMapper.h"
#include "interpreter.h"
#ifdef EXT_PROMPT
#include "colors.h"
#include "redirections.h"
#include "directoryStack.h"
#endif

// BONUS Lab 1! Add a prompt that displays the current directory
void displayCurrentDirectory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("[%s]$ ", cwd);
    } else {
        perror("getcwd");
    }
}


// Handle SIGINT signal (BONUS Lab1! exit the shell and remove colouring)
#ifdef EXT_PROMPT
void handle_signal(int signum) {
    if (signum == SIGINT) {
        printf(COLOR_RESET);
        printf("\n");
        exit(signum);
    }
}
#endif


// Here it all begins
int main(int argc, char *argv[]) {
    // BONUS Lab 2!
    #ifdef EXT_PROMPT
    if (argc == 2) {
        setAsInFile(argv[1]);
    }
    #endif

    // BONUS Lab 1! Add a prompt that displays the current directory
    #ifdef EXT_PROMPT
    signal(SIGINT, handle_signal);
    #endif
    // Disable input and output buffering to avoid out-of-order prints in Themis
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char *inputLine;
    List tokenList, parseList, listBackup;

    State state = newDefaultState();

    BuiltinMapper mapper = newBuiltinMapper();
    addBuiltinMapper(&state, &mapper);

    #ifdef EXT_PROMPT
    DirectoryStack stack = newDirectoryStack();
    addDirectoryStack(&state, &stack);
    #endif

    // Main loop, goes line by line
    while (state.isRunning) {
        // BONUS Lab 1! Add a prompt that displays the current directory
        #ifdef EXT_PROMPT
        printf(state.colourCode);
        if (argc != 2) {
            displayCurrentDirectory();
        }
        #endif

        // Get the input line and tokenize
        inputLine = readInputLine();
        tokenList = getTokenList(inputLine);
        free(inputLine);
        
        // Make a backup for freeing the token list later
        listBackup = tokenList;

        // Parse the input line. Parse list will be NULL if the input line is valid
        parseList = tokenList;
        bool parsedSuccessfully = parseInputLine(&parseList);

        if (parseList == NULL && parsedSuccessfully) {
            // Interpret the input line
            if (!interpretInputLine(&tokenList, &state)) {
                printError(state, "Error: command not found!");
                freeTokenList(listBackup);
                freeBuiltinMapper(mapper);
                #ifdef EXT_PROMPT
                freeDirectoryStack(stack);
                #endif
                freeState(state);
                exit(127);
            };
        } else {
            printError(state, "Error: invalid syntax!");
        }
        freeTokenList(listBackup);

        // BONUS Lab 2! put newlines between command outputs to make it more readable
        #ifdef EXT_PROMPT
        if (argc == 2 && state.isRunning) {
            putchar('\n');
        }
        #endif
    }
    freeBuiltinMapper(mapper);
    #ifdef EXT_PROMPT
    freeDirectoryStack(stack);
    #endif
    freeState(state);
    
    return 0;
}