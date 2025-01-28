#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "list.h"

// Returns a new empty list
List newEmptyList() {
    return NULL;
}

// Returns a new list with a single value
List newList(int value) {
    List new = malloc(sizeof(struct ListNode));
    new->value = value;
    new->next = NULL;
    return new;
}

// scans a list until it hits a -1
List scanList() {
    int c;
    scanf("%d", &c);
    if (c == -1) {
        return newEmptyList();
    }
    List this = newList(c);
    this->next = scanList();
    return this;
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
 * The function printList prints the values in a list, separated by commas.
 * @param li the input list to be printed.
 */
void printList(List li) {
    printf("Printing list: ");
    if (li == NULL) return;
    printf("%d", li->value);
    li = li->next;
    while (li != NULL) {
        printf(", %d", li->value);
        li = li->next;
    }
    printf("\n");
}

/**
 * The function freelist frees the memory of the nodes of the list, and of the strings
 * in the nodes.
 * @param li the starting node of a list.
 */
void freeList(List li) {
    if (li == NULL) {
        return;
    }
    freeList(li->next);
    free(li);
}

// Returns the first element of the list
int getFirst(List l) {
    return l->value;
}
