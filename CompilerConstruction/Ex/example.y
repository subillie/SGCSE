%{
#include <stdio.h>
#include <stdlib.h>
extern FILE *yyin;
void yyerror(char *s);
int yylex(void);
%}

%union {
    int n;
    char *s;
}
%token <n> NUM
%token <s> ID
%token PLUS MULT
%start E

%%

E: E PLUS T
 | T;

T: T MULT F
 | F;

F: NUM
 | ID ;

%%

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <input file>\n", argv[0]);
        exit(1);
    }

    if (NULL == (yyin = fopen(argv[1], "r"))) {
        printf("Failed to open %s\n", argv[1]);
        exit(1);
    }

    yyparse();
}

void yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
}
