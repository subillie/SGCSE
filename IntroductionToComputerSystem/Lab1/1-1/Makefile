main.bin: copyLSB.o main.o
	gcc copyLSB.o main.o -o main.bin

copyLSB.o: copyLSB.h copyLSB.c
	gcc -c copyLSB.c

main.o: copyLSB.h main.c
	gcc -c main.c

clean:
	rm -f *.o *.bin

