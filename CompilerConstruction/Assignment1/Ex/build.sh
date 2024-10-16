#!/bin/bash
bison -d example.y
flex example.l
gcc -o main.bin example.tab.c lex.yy.c
