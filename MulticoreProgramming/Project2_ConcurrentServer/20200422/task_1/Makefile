# CC = gcc
# CFLAGS = -O2 -Wall
# LDLIBS = -lpthread

# all: multiclient stockclient stockserver

# multiclient: multiclient.c csapp.c csapp.h
# stockclient: stockclient.c csapp.c csapp.h
# stockserver: stockserver.c csapp.c csapp.h

# clean:
# 	rm -rf *~ multiclient stockclient stockserver *.o

all:
	gcc -O2 -Wall -lpthread -o multiclient multiclient.c csapp.c csapp.h
	gcc -O2 -Wall -lpthread -o stockclient stockclient.c csapp.c csapp.h
	gcc -O2 -Wall -lpthread -o stockserver stockserver.c csapp.c csapp.h