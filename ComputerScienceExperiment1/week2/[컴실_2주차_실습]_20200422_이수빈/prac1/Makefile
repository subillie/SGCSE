CC = gcc
CFLAGS = -W -g

TARGET = animal
OBJS = dog.o blackcow.o turtle.o main.o

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm $(TARGET) $(OBJS)

.PHONY : clean
