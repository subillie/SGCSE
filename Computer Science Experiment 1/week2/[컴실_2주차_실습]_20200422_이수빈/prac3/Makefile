CC = gcc
CFLAGS = -W -g

TARGET = fmt
OBJS = main.o output.o stringM.o

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm $(TARGET) $(OBJS)

.PHONY : clean