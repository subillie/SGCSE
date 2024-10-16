#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "variable.h"

// Add a node that respresents variable definition "x = v", at the front of a
// linked list. Here, 'head' is the first node of the input linked list. The
// return value is the first node of the updated linked list.
VarNode* add_var(char *x, int v, VarNode *head) {
    VarNode* node = (VarNode*)malloc(sizeof(VarNode));
    node->var = x;
    node->val = v;
    node->next = head;
    return node;
}

// Lookup and return the value of variable 'x' stored in a linked list of
// variables. Here, 'head' is the first node of the input linked list.
int lookup_var(VarNode *head, char *x) {
    while (head) {
        if (strcmp(x, head->var) == 0)
            return head->val;
        head = head->next;
    }
    printf("Uninitialized variable: %s\n", x);
    exit(1);
}
