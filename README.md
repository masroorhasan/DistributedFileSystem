<h1>Client</h1>

<h2>RPC Files:</h2>

ece454rpc_types.h
helper.c
client_stub.c

<h2>FS Files:</h2>

ece454_fs.h
fsOtherIncludes.h
ece454_fs_client.c

<h2>Sample:</h2>

sample_client_app.c

<h2>Compile:</h2>

gcc helper.c client_stub.c ece454_fs_client.c sample_client_app.c


<h1>Server </h1>

<h2>RPC Files:</h2>

ece454rpc_types.h
helper.c
mybind.c
server_stub.c

<h2>FS Files:</h2>

ece454_fs.h
fsOtherIncludes.h
ece454_fs_server

<h2>Application:</h2>

sample_server_app.c

<h2>Compile:</h2>

gcc helper.c mybind.c server_stub.c ece454_fs_server.c sample_server_app.c
