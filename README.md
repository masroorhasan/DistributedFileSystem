<h1>Client</h1>

<h3>RPC Files:</h3>

ece454rpc_types.h<br>
helper.c<br>
client_stub.c<br>

<h3>FS Files:</h3>

ece454_fs.h<br>
fsOtherIncludes.h<br>
ece454_fs_client.c<br>

<h3>Sample:</h3>

sample_client_app.c

<h3>Compile:</h3>

gcc helper.c client_stub.c ece454_fs_client.c sample_client_app.c


<h1>Server </h1>

<h3>RPC Files:</h3>

ece454rpc_types.h<br>
helper.c<br>
mybind.c<br>
server_stub.c<br>

<h3>FS Files:</h3>

ece454_fs.h<br>
fsOtherIncludes.h<br>
ece454_fs_server<br>

<h3>Application:</h3>

sample_server_app.c

<h3>Compile:</h3>

gcc helper.c mybind.c server_stub.c ece454_fs_server.c sample_server_app.c
