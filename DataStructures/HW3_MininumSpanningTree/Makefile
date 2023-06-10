all:  # compile
	gcc cse3080_hw3_20200422.c -o hw3

clean:  # remove only the binary file
	rm -f hw3

fclean: clean  # remove all output files
	rm -f hw3_result.txt

re: fclean all

.PHONY: all clean fclean re