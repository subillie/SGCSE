%option noyywrap

%{
#include "example.tab.h"
%}

dig [0-9]
let [A-Za-z]

%%

"+"                         { return PLUS; }
"*"                         { return MULT; }
{dig}+                      { yylval.n = atoi(yytext);
                              return NUM; }
({let}|_)({let}|{dig}|_)*   { yylval.s = strdup(yytext);
                              return ID; }
[ \t\n]+                    { /* Skip whitespace. */ }

%%
