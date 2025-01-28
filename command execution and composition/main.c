#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

#include "scanner.h"
#include "shell.h"
#include "state.h"
#include "builtin.h"
#include "interpreter.h"
#include "colours.h"

// BONUS! Add a prompt that displays the current directory
#ifdef EXT_PROMPT
void displayCurrentDirectory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("[%s]$ ", cwd);
    } else {
        perror("getcwd");
    }
}
#endif

// Print an error message (BONUS! in red)
void printError(State state, char *message) {
    #ifdef EXT_PROMPT
    printf("\033[0;31m"); // Red
    #endif

    printf("%s\n", message);

    #ifdef EXT_PROMPT
    printf("%s", state.colourCode); // Reset
    #endif
}

// Handle SIGINT signal (BONUS! exit the shell and remove colouring)
#ifdef EXT_PROMPT
void handle_signal(int signum) {
    if (signum == SIGINT) {
        printf(COLOUR_RESET);
        printf("\n");
        exit(signum);
    }
}
#endif


// Here it all begins
int main(int argc, char *argv[]) {
    // BONUS! Add a prompt that displays the current directory
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

    // Main loop, goes line by line
    while (state.isRunning) {
        // BONUS! Add a prompt that displays the current directory
        #ifdef EXT_PROMPT
        displayCurrentDirectory();
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
            if (!interpretInputLine(&tokenList, mapper, &state)) {
                printError(state, "Error: command not found!");
                freeTokenList(listBackup);
                freeBuiltinMapper(mapper);
                freeState(state);
                exit(127);
            };
        } else {
            printError(state, "Error: invalid syntax!");
        }
        freeTokenList(listBackup);
    }
    freeBuiltinMapper(mapper);
    freeState(state);
    
    return 0;
}