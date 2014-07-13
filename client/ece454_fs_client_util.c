#include "ece454_fs_client.h"

/*
 * Serilaizes an FSDIR to return_type so it can be sent
 * back to a client.
 */
extern return_type serializeFSDIR(FSDIR* folder) {
    return_type r;
    return r;
}

/*
 * Deserializes an FSDIR from the client so a server can
 * operate on it.
 */
extern FSDIR* deserializeFSDIR(const int nparams, arg_type *a) {
    FSDIR* dir;
    return dir;
}
