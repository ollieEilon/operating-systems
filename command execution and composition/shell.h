#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <stdbool.h>
#include "scanner.h"

bool parseInputLine(List *lp);
bool acceptToken(List *lp, char *ident);
bool isOperator(char *token);
bool parseChain(List *lp);

#endif