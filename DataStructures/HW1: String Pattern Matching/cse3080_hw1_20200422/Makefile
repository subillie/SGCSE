TARGET_KMP = hw1_kmp
TARGET_NAIVE = hw1_naive

SRC_KMP = hw1_kmp.c
SRC_NAIVE = hw1_naive.c
OBJ_KMP = $(SRC_KMP:.c=.o)
OBJ_NAIVE = $(SRC_NAIVE:.c=.o)

CC = gcc
RM = rm -f

all : $(TARGET_KMP) $(TARGET_NAIVE)

%.o: %.c
	$(CC) -I./ -c $< -o $@

$(TARGET_KMP): $(OBJ_KMP)
	$(CC) -o $(TARGET_KMP) $(OBJ_KMP)
$(TARGET_NAIVE): $(OBJ_NAIVE)
	$(CC) -o $(TARGET_NAIVE) $(OBJ_NAIVE)

clean :
	$(RM) $(OBJ_KMP) $(OBJ_NAIVE)

fclean : clean
	$(RM) $(TARGET_KMP) $(TARGET_NAIVE)
	$(RM) result_*.txt

re : fclean all

.PHONY: all clean fclean re
