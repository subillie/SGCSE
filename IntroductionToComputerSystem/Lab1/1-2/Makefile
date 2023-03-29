main.bin: absVal.o main.o
	gcc absVal.o main.o -o main.bin

absVal.o: absVal.h absVal.c
	gcc -c absVal.c

main.o: absVal.h main.c
	gcc -c main.c

clean:
	rm -f *.o *.bin

