main.bin: conditional.o main.o
	gcc conditional.o main.o -o main.bin

conditional.o: conditional.h conditional.c
	gcc -c conditional.c

main.o: conditional.h main.c
	gcc -c main.c

clean:
	rm -f *.o *.bin

