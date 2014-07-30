objects := $(patsubst %.c,%.o,$(wildcard *.c))
includes := $(wildcard *.h)

server::
	gcc serv/mybind.c serv/helper.c serv/server_stub.c serv/ece454_fs_server_util.c serv/ece454_fs_server.c serv/server.c -o serv_executeable

client-api.a: sample_client_app.o
	ar r client-api.a helper.o client_stub.o ece454_fs_client.o

sample_client_app.o:
	gcc -c client/helper.c client/client_stub.c client/ece454_fs_client.c

clean:
	rm -rf server/a.out server/*.o server/core server/*.a server/*_app client/a.out client/*.o client/core client/*.a client/*_app server_exectureable client-api.a *.o *.a *_app

