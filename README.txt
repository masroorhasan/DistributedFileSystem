Client RPC Files:

ece454rpc_types.h
helper.c
client_stub.c
sample_client_app.c

Compile Server:

gcc helper.c mybind.c server_stub.c ece454_fs_server.c sample_server_app.c


Client FS Files:

ece454_fs.h
fsOtherIncludes.h
ece454_fs_client.c

Compile Client:

gcc helper.c client_stub.c ece454_fs_client.c sample_client_app.c
