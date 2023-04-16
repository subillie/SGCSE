TARGET = phase3

SRC = csapp.c myshell.c signal.c evaluator.c executer.c parser.c pipe.c jobs.c
OBJ = $(SRC:.c=.o)

CC = gcc
RM = rm -f

all : $(TARGET)
	@printf "/* Run phase3! */\n"

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