NAME = mp2_20200422
CC = g++ -std=c++98
RM = rm -f

SRCS = main.cc Sort.cc
OBJS = $(SRCS:.cc=.o)

all: $(NAME)

.c.o:
	@$(CC) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $@
	@echo "Ready to run!"

clean:
	@$(RM) $(NAME) $(OBJS)

re: clean all

.PHONY: all clean re
