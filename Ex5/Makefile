OBJS	= server.o stack.o
SOURCE	= server.c stack.c

OUT	= server
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread

all: server client 

test: test.o stack.o
	$(CC) stack.o test.o -o test

server: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

client: client.o
	$(CC) client.o -o client 

server.o: server.c
	$(CC) $(FLAGS) server.c

stack.o: stack.c
	$(CC) $(FLAGS) stack.c

client.o: client.c
	$(CC) -c client.c


test.o: test.c
	$(CC) test.c -c
clean:
	rm -f $(OBJS)  $(OUT) client *.o test