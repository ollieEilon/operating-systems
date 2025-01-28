#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "parser.h"

bool parseInputLine(List *lp);
bool acceptToken(List *lp, char *ident);
bool isOperator(char *token);
bool parseChain(List *lp);

#endif