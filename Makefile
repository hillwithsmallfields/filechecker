all: filecheck

filechecker.o: filecheck.c
	gcc -g -c -o filecheck.o filecheck.c

filechecker: filecheck.o
	gcc -o filecheck filecheck.o
