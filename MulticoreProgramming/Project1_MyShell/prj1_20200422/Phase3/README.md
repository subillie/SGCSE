# [ Project Phase III: Run Processes in Background ]

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

## PHASE3
Shell supports the notion of `job control`, which allows users to move jobs back and forth between background and foreground, and to change the process state (running, stopped, or terminated) of the processes in a job. <br> Besides, shell must start a command in background if an `'&'` is given in it, and must also provide the commands `'jobs', 'bg', 'fg', 'kill'`.

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
| job.c			| Functions used when handle jobs |
