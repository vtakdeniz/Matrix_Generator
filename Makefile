 CC=gcc

all: myshell

myshell: matrixcalc.c  
	$(CC) -o myshell matrixcalc.c
