main.bin: bitset.o main.o
	gcc bitset.o main.o -o main.bin

bitset.o: bitset.h bitset.c
	gcc -c bitset.c

main.o: bitset.h main.c
	gcc -c main.c

clean:
	rm -f *.o *.bin

