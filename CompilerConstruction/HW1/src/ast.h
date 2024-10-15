#ifndef AST_HEADER
#define AST_HEADER

#include "variable.h"

typedef enum {
    Number,
    Identifier,
    Add,
    Sub,
    Mul,
    Div,
    Neg
} AST_KIND;

struct AST {
    // Specifies what kind of expression is represented by this AST.
    AST_KIND kind;
    // 'num' is used only when 'kind' is Number.
    int num;
    // 'id' is used only when 'kind' is Identifier.
    char *id;
    // 'left' and 'right' are children nodes that will be used when 'kind' is a
    // binary or unary operation. If 'kind' is Add/Sub/Mull/Div, both 'left' and
    // 'right' are be used. If 'kind' is Neg, only 'left' field is used.
    struct AST *left;
    struct AST *right;
};

typedef struct AST AST;

// Create and return a pointer to an AST node that represents integer 'n'.
AST* make_num_ast(int n);

// Create and return a pointer to an AST node that represents identifier 's'.
AST* make_id_ast(char *s);

// Create and return a pointer to an AST node that represents a negate operation
// of 'oprnd'.
AST* make_neg_ast(AST *oprnd);

// Create and return a pointer to an AST node that represents a binary operation
// with 'oprnd_1' and 'oprnd_2'.
AST* make_binop_ast(AST_KIND kind, AST *oprnd_1, AST *oprnd_2);

// Return the integer value obtained by evaluating the numeric expression
// represented by the input AST.
int eval_ast(VarNode *vars, AST* ast);

#endif
