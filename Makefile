all: filecheck

filechecker.o: filecheck.c
	gcc -c -o filecheck.o filecheck.c

filechecker: filecheck.o
	gcc -o filecheck filecheck.o
