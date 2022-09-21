all: server client

server: server.o
	gcc -std=gnu99 server.c -o server -pthread
	
client: client.o
	gcc -std=gnu99 client.c -o client

.PHONY: clean all

clean:
	rm -f *.o server client
