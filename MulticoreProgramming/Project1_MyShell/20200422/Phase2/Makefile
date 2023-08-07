TARGET = myshell

SRC = csapp.c myshell.c signal.c evaluator.c executor.c parser.c pipe.c
OBJ = $(SRC:.c=.o)

CC = gcc
RM = rm -f

all : $(TARGET)
	@printf "/* Run myshell! */\n"

%.o: %.c
	@$(CC) -I./ -c $< -o $@

$(TARGET): $(OBJ)
	@$(CC) -o $(TARGET) $(OBJ)

clean :
	@$(RM) $(OBJ)
	@printf "/* Removed object files */\n"

fclean : clean
	@$(RM) $(TARGET)
	@printf "/* Removed executable binary file */\n"

re : fclean all

.PHONY: all clean fclean re