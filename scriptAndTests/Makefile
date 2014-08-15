OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))

all: server client-api.a

server: libstubs.a server.o
	gcc server.o -L. -lstubs -o fsServer

libstubs.a: server_stub.o client_stub.o helper.o mybind.o
	ar r libstubs.a server_stub.o client_stub.o helper.o mybind.o

client-api.a: server_stub.o client_stub.o helper.o mybind.o fakeclient-api.o
	ar r libclient-api.a server_stub.o client_stub.o helper.o mybind.o fakeclient-api.o

$(OBJECTS): %.o: %.c ece454_fs.h ece454rpc_types.h
	gcc -c -g $< -o $@

clean:
	rm -rf a.out *.o core *.a fsServer
