# [ Project Phase II: Redirection and Pipe ]

## MAKEFILE

1. How to compile
```bash
> make		# compile
> ./myshell	# execute
```

2. Makefile options
```
- make clean	: remove all the object files
- make fclean	: remove all the object files and the executable binary file
- make re		: fclean and then make
```

## PHASE2
A `pipe` allows us to use two or more commands in a shell so that the output of one command serves as the input to the next. <br> To handle multiple pipes, the `dup2()` system call is used, and functions in pipe.c have been implemented for the first pipe, the last pipe, and the pipes between them.

## FILES
|||
|:---:|:---|
| csapp.{c,h}	| CS:APP3e functions |
| myshell.{c,h}	| Main function and the header including all the C files |
| signal.c		| Signal handlers |
| evaluator.c	| Evaluate command line by parser and executer |
| parser.c		| Parse command line |
| executer.c	| Execute command line in the way of builtin commands or extern functions |
| pipe.c		| Functions used when command line has pipe |
