CC = gcc
CFLAGS = -W -g
TARGET = main
OBJS = main.o CountDigits.o Print.o

$(TARGET) : $(OBJS)
	$(CC) $(CLFAGS) -o $(TARGET) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.o CountDigits.o Print.o : Header.h

clean :
	rm $(TARGET) $(OBJS)
.PHONY : clean