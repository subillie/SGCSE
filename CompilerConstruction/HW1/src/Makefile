../main.bin: variable.h variable.c ast.h ast.c prog.y prog.l
	bison -d prog.y
	flex prog.l
	gcc -o ../main.bin prog.tab.c lex.yy.c variable.c ast.c

clean:
	rm -f prog.tab.* lex.yy.c ../main.bin
