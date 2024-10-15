#ifndef VARIABLE_HEADER
#define VARIABLE_HEADER

// Linked list that contains information about variable definition.
struct VarNode {
    char *var; // Name of a variable
    int val; // Value of a variable
    struct VarNode *next; // Next node for linked list
};

typedef struct VarNode VarNode;

// Add a node that respresents variable definition "x = v", at the front of a
// linked list. Here, 'head' is the first node of the input linked list. The
// return value is the first node of the updated linked list.
VarNode* add_var(char *x, int v, VarNode *head);

// Lookup and return the value of variable 'x' stored in a linked list of
// variables. Here, 'head' is the first node of the input linked list.
int lookup_var(VarNode *head, char *x);

#endif
