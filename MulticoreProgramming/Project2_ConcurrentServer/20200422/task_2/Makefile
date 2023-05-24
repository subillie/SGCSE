CC = gcc
CFLAGS = -O2 -Wall
LDLIBS = -pthread

SRCS_MULTICLIENT = multiclient.c csapp.c
SRCS_STOCKCLIENT = stockclient.c csapp.c
SRCS_STOCKSERVER = stockserver.c server_utils.c server_bst.c csapp.c

all: multiclient stockclient stockserver

multiclient: $(SRCS_MULTICLIENT)
	$(CC) $(CFLAGS) $(LDLIBS) -o multiclient $(SRCS_MULTICLIENT)
stockclient: $(SRCS_STOCKCLIENT)
	$(CC) $(CFLAGS) $(LDLIBS) -o stockclient $(SRCS_STOCKCLIENT)
stockserver: $(SRCS_STOCKSERVER)
	$(CC) $(CFLAGS) $(LDLIBS) -o stockserver $(SRCS_STOCKSERVER)

clean:
	rm -rf *~ multiclient stockclient stockserver *.o
