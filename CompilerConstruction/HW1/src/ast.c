#include <stdio.h>
#include <stdlib.h>
#include "variable.h"
#include "ast.h"

// Create and return a pointer to an AST node that represents integer 'n'.
AST* make_num_ast(int n) {
    AST* ast = (AST*)malloc(sizeof(AST));
    ast->kind = Number;
    ast->num = n;
    ast->id = NULL;
    ast->left = NULL;
    ast->right = NULL;
    return ast;
}

// Create and return a pointer to an AST node that represents identifier 's'.
AST* make_id_ast(char *s) {
    AST* ast = (AST*)malloc(sizeof(AST));
    ast->kind = Identifier;
    ast->num = 0;
    ast->id = strdup(s); // 주어진 문자열 s를 기반으로 새로운 AST node 생성
    ast->left = NULL;
    ast->right = NULL;
    return ast;
}

// Create and return a pointer to an AST node that represents a negate operation
// of 'oprnd'.
AST* make_neg_ast(AST *oprnd) {
    AST* ast = (AST*)malloc(sizeof(AST));
    ast->kind = Neg;
    ast->num = 0;
    ast->id = NULL;
    ast->left = oprnd;
    ast->right = NULL;
    return ast;
}

// Create and return a pointer to an AST node that represents a binary operation
// with 'oprnd_1' and 'oprnd_2'.
AST* make_binop_ast(AST_KIND kind, AST *oprnd_1, AST *oprnd_2) {
    AST* ast = (AST*)malloc(sizeof(AST));
    ast->kind = kind; // 연산 종류
    ast->num = 0;
    ast->id = NULL;
    ast->left = oprnd_1; // 두 개의 피연산자를 자식 노드에 각각 저장
    ast->right = oprnd_2;
    return ast;
}

// Return the integer value obtained by evaluating the numeric expression
// represented by the input AST.
int eval_ast(VarNode *vars, AST* ast) {
    switch (ast->kind) {
        case Number:
            return ast->num;
        case Identifier:
            return lookup_var(vars, ast->id);
        case Add:
            return eval_ast(vars, ast->left) + eval_ast(vars, ast->right);
        case Sub:
            return eval_ast(vars, ast->left) - eval_ast(vars, ast->right);
        case Mul:
            return eval_ast(vars, ast->left) * eval_ast(vars, ast->right);
        case Div: {
            int ast_right = eval_ast(vars, ast->right);
            if (ast_right == 0) {
                printf("Error: Division by zero\n");
                exit(1);
            }
            return eval_ast(vars, ast->left) / ast_right;
        }
        case Neg:
            return -eval_ast(vars, ast->left);
        default:
            printf("Error: Unknown expression\n");
            exit(1);
    }
    return 0;
}
