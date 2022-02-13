CC = gcc
CFLAGS  = -g

default : myshell

myshell: myshell_parser.o myshell.o
	$(CC) $(CFLAGS) -o myshell myshell_parser.o myshell.o

myshell_parser.o: myshell_parser.c myshell_parser.h
	$(CC) $(CFLAGS) -c myshell_parser.c

myshell.o: myshell.c myshell.h
	$(CC) $(CFLAGS) -c myshell.c

clean: 
	$(RM) myshell *.o *~