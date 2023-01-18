CC = gcc
CFLAGS = -W -g

OBJS = main.o countDigits.o print.o
TARGET = subin

$(TARGET) : $(OBJS)
	$(CC) $(CLFAGS) -o $(TARGET) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm $(TARGET) $(OBJS)

.PHONY : clean
