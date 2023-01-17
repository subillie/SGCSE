CC = gcc
CFLAGS = -W -g

OBJS = main.o CountDigits.o Print.o
TARGET = subin

$(TARGET) : $(OBJS)
	$(CC) $(CLFAGS) -o $(TARGET) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f $(TARGET) $(OBJS)
.PHONY : clean