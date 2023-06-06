# all:
# 	g++ -o -Wall distvec_20200422 distvec_20200422.cc 
# 	g++ -o -Wall linkstate_20200422 linkstate_20200422.cc

all:
	g++ -Wall distvec_20200422.cc -o distvec_20200422
	g++ -Wall linkstate_20200422.cc -o linkstate_20200422

dv :
	g++ -Wall distvec_20200422.cc -o distvec_20200422

ls:
	g++ -Wall linkstate_20200422.cc -o linkstate_20200422

clean:
	rm -f distvec_20200422 linkstate_20200422

fclean: clean
	rm -f output_dv.txt output_ls.txt

re: fclean all