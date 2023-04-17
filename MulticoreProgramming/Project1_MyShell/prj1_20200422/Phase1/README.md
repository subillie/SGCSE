# [ Project Phase I: Building and Testing My Shell ]

## MAKEFILE

1. How to compile
```bash
> make		# compile
> ./phase1	# execute
```

2. Makefile options
```
- make clean	: remove all the object files
- make fclean	: remove all the object files and the executable binary file
- make re : fclean and then make
```

## PHASE1
Shell should be able to execute the `basic internal shell commands` such as cd, ls, mkdir, rmdir, touch, cat, echo, history, !! !#, exit. <br> For extern functions, except for builtins, commands should be executed by the child process created via forking by the parent process.

## FILES
|||
|:---:|:---|
| csapp.{c,h}	| CS:APP3e functions |
| myshell.{c,h}	| Main function and the header including all the C files |
| signal.c		| Signal handlers |
| evaluator.c	| Evaluate command line by parser and executer |
| parser.c		| Parse command line |
| executer.c	| Execute command line in the way of builtin commands or extern functions |
