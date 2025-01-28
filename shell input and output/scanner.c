#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

#include "scanner.h"


/**
 * Reads an inputline from stdin.
 * @return a string containing the inputline.
 */
char *readInputLine() {
    int strLen = INITIAL_STRING_SIZE;
    int c = getchar();
    int i = 0;

    char *s = malloc((strLen + 1) * sizeof(*s));
    assert(s != NULL);

    bool quoteStarted = false;
    // Ensure that the EOF gets read as a character, but then it stops scanning
    bool reachedEOF = false;
    while ((c != '\n' || quoteStarted) && !reachedEOF) { // Ensure that newlines in strings are accepted
        
        if (c == '\"') {
            quoteStarted = !quoteStarted;
        }
        if (c == EOF) {
            reachedEOF = true;
            c = "\x1A"[0]; // set c to EOF character
        }

        s[i++] = c;

        if (i >= strLen) { // Resize the string if necessary
            strLen = 2 * strLen;
            s = realloc(s, (strLen + 1) * sizeof(*s));
            assert(s != NULL);
        }
        c = getchar();
    }

    s[i] = '\0';
    return s;
}

/**
 * The function isOperatorCharacter checks whether the input paramater \param c is an operator.
 * EOF counts aswell, as it would stop options from getting parsed
 * @param c input character.
 * @return a bool denoting whether \param c is an operator.
 */
bool isOperatorCharacter(char c) {
    return c == '&' || c == '|' || c == ';' || c == '<' || c == '>' || c == EOF;
}

// BONUS Lab 2! Detect a super redirect node (n>)
/**
 * The function isSuperRedirect checks whether the input paramater \param s is a super redirect (n>).
 * @param s input string.
 * @return a bool denoting whether \param s is a super redirect.
 */
bool isSuperRedirect(char *s) {
    return s[0] == 'n' && s[1] == '>';
}

/**
 * Reads an identifier in string \param s starting at index \param start.
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a pointer to the start of the identifier string
 */
char *matchIdentifier(char *s, int *start) {
    int strLen = INITIAL_STRING_SIZE;
    int pos = 0, offset = 0;

    char *ident = malloc((strLen + 1) * sizeof(*ident));
    assert(ident != NULL);

    bool quoteStarted = false;
    size_t lenS = strlen(s);
    while ((*start + offset <= lenS && !isspace(s[*start + offset]) && !isOperatorCharacter(s[*start + offset])) || quoteStarted) { // Ensure that whitespace in strings is accepted
        if (s[*start + offset] == '\"') { // Strip the quotes from the input before storing in the identifier
            quoteStarted = !quoteStarted;
            offset++;
            continue;
        }
        ident[pos++] = s[*start + offset++];
        if (pos >= strLen) { // Resize the string if necessary
            strLen = 2 * strLen;
            ident = realloc(ident, (strLen + 1) * sizeof(*ident));
            assert(ident != NULL);
        }
    }
    ident[pos] = '\0';
    *start = *start + offset;
    return ident;
}

/**
 * The function newEmptyNode makes a new empty node for the token list.
 * @return a list node that contains no token.
 */
List newEmptyNode() {
    List this = malloc(sizeof(*this));
    assert(this != NULL);
    this->next = NULL;
    this->t = NULL;
    return this;
}

/**
 * The function newNode makes a new node for the token list and fills it with the token that
 * has been read. Precondition: !isspace(a[*ip]).
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a list node that contains the current token.
 */
List newNode(char *s, int *start) {
    List node = newEmptyNode();
    node->t = matchIdentifier(s, start);
    return node;
}

/**
 * Reads an operator in string \param s starting at index \param start.
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a pointer to the start of the operator string.
 */
char *matchOperator(char *s, int *start) {
    int strLen = 2; // the operator consists of *at most* 2 characters
    int pos = 0, offset = 0;

    char *op = malloc((strLen + 1) * sizeof(*op));
    assert(op != NULL);

    while (isOperatorCharacter(s[*start + offset])) {
        op[pos++] = s[*start + offset++];
    }
    op[pos] = '\0';
    *start = *start + offset;
    return op;
}

/**
 * The function newOperatorNode makes a new operator node for the token list and fills it with the token that
 * has been read. Precondition: !isspace(a[*ip]).
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a list node that contains the current token.
 */
List newOperatorNode(char *s, int *start) {
    List node = newEmptyNode();
    node->t = matchOperator(s, start);
    return node;
}

// BONUS Lab 2! Create a new super redirect node (n>)
/**
 * The function newSuperRedirectNode makes a new super redirect node for the token list and fills it with the token that
 * has been read. Precondition: !isspace(a[*ip]).
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a list node that contains the current token.
 */
List newSuperRedirectNode(char *s, int *start) {
    List node = newEmptyNode();

    // Manually set the token to "n>"
    node->t = malloc(3 * sizeof(*node->t));
    assert(node->t != NULL);
    node->t[0] = 'n';
    node->t[1] = '>';
    node->t[2] = '\0';
    *start += 2;
    return node;
}

/**
 * The function tokenList reads an array and puts the tokens that are read in a list.
 * @param s input string.
 * @return a pointer to the beginning of the list.
 */
List getTokenList(char *s) {
    List lastNode = NULL;
    List node = NULL;
    List tl = NULL;
    int i = 0;
    int length = strlen(s);
    while (i < length) {
        if (isspace(s[i])) { // spaces are skipped
            i++;
        } else {
            if (isOperatorCharacter(s[i])) { // operator
                node = newOperatorNode(s, &i);
            #ifdef EXT_PROMPT
            } else if (isSuperRedirect(s + i)) {
                node = newSuperRedirectNode(s, &i);
            #endif
            } else { // identifier
                node = newNode(s, &i);
            }
            if (lastNode == NULL) { // there is no list yet
                tl = node;
            } else { // a list already exists; add current node at the end
                (lastNode)->next = node;
            }
            lastNode = node;
        }
    }
    return tl;
}

/**
 * Checks whether list \param l is empty.
 * @param l input list.
 * @return a bool denoting whether \param l is empty.
 */
bool isEmpty(List l) {
    return l == NULL;
}

/**
 * The function printList prints the tokens in a token list, separated by commas.
 * @param li the input list to be printed.
 */
void printList(List li) {
    if (li == NULL) return;
    printf("\"%s\"", li->t);
    li = li->next;
    while (li != NULL) {
        printf(", \"%s\"", li->t);
        li = li->next;
    }
    printf("\n");
}

/**
 * The function freeTokenlist frees the memory of the nodes of the list, and of the strings
 * in the nodes.
 * @param li the starting node of a list.
 */
void freeTokenList(List li) {
    if (li == NULL) {
        return;
    }
    free(li->t);
    freeTokenList(li->next);
    free(li);
}