Client API:

This API enforces entry level consistency on a per file basis.

1. A file is locked on a call to fsOpen()
2. A file is unlocked on a call to fsClose()
3. fsRemove() requires any preceeding locks on a file
   to be released before it can execute

fsMount() and fsUnmount() support multiple mounts.

Server:

The server maintains a priority queue on a per file basis
of open and remove requests in the order in which they
are received.

A client will poll the server and attempt to gain a lock if another
client has the a lock on the file being requested.
