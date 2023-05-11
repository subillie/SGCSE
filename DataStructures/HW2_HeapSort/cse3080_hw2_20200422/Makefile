TARGET = hw2
SRC = heapsort.c
OBJ = $(SRC:.c=.o)

CC = gcc
RM = rm -f

all : $(TARGET)

.c.o :
	@$(CC) -c $< -o $@

$(TARGET) : $(OBJ)
	@$(CC) -o $@ $^
	@printf "Successfully compiled!\n"

clean :
	@$(RM) $(TARGET)
	@$(RM) $(OBJ)
	@printf "Successfully removed!\n"

./Phony : all clean
