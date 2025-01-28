#ifndef INC_2_LIST_H
#define INC_2_LIST_H

#include <stdbool.h>

// A linked list of integers

typedef struct ListNode *List;

typedef struct ListNode {
    int value;
    List next;
} ListNode;

List scanList();
bool isEmpty(List l);
void printList(List l);
void freeList(List l);
int getFirst(List l);


#endif //INC_2_LIST_H