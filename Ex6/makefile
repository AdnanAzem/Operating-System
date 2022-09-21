cc = gcc
gpp = g++
all: clientsTest Guard singleton SelectClient PollServer main1 client lib.so
SelectClient: SelectClient.o
	$(cc) $<  -o SelectClient -pthread
main1: main1.o Active_Object.o Queue.o  
	$(cc) main1.o Active_Object.o Queue.o -o main1 -pthread
client: client.o
	$(cc) $< -o client
Guard: Guard.o
	$(gpp) Guard.cpp -o Guard -pthread
lib.so: Queue.o 
	$(cc) --shared -fPIC -pthread Queue.o -o lib.so
singleton: singleton.o
	$(gpp) singleton.cpp -o singleton -pthread
PollServer: PollServer.o
	$(cc) $<  -o PollServer -pthread
%.o: %.c
	$(cc) -c $< -o $@
clean:
	rm -f *.o  SelectClient clientsTest Active_Object PollServer main1 client lib.so Guard Queue singleton
.PHONY: clean all