FLAGS = -g -Wall
CC = gcc
.PHONY: clean

all: main server

main: main.o
	$(CC) $(FLAGS) -o main main.o

main.o: main.c shell.h ANSI-color-codes.h
	$(CC) $(FLAGS) -c main.c

server: server.o
	$(CC) $(FLAGS) -o server server.o

server.o: server.c server.h
	$(CC) $(FLAGS) -c server.c

clean:
	rm -f *.o *.a *.so main server