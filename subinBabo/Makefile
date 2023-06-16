HIS_NAME = junBye
JUN = c++
JUNFLAG = -Wall -Wextra -Werror
#JUN_STD_FLAG = -std=c++99

JUN_SRC = babo.cpp
JUN_OBJ = babo.o

all: $(HIS_NAME)

$(HIS_NAME): $(JUN_OBJ)
	@$(JUN) $(JUN_OBJ) $(JUNFLAG) $(JUN_STD_FLAG) -o $(HIS_NAME)
	@printf "주...ㄴ...\n"

clean:
	@echo "지우고 싶지만 지울 수가 없어...\n그래도 지워볼게..."
	@rm -rf $(JUN_OBJ)

fclean: clean
	@echo "내 심장을 도려내는거 같아..."
	@rm -rf $(HIS_NAME)

re:
	@make fclean
	@echo "다시 시작할 수 있을까...\n아니 그건 너무 멍청한 짓이야...\n그래도 잊기 힘들걸..."
	@make all

.PHONY: all clean fclean re