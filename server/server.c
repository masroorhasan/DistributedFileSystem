#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ece454rpc_types.h"
#include "ece454_fs_server.h"

#if 0
#define _DEBUG_1_
#endif


extern void printRegisteredProcedures();


int main() {

    // Register file system procedures
    register_procedure("fsMount", 2, fsMount); // Should be 3 parameters
    register_procedure("fsUnmount", 1, fsUnmount);
    register_procedure("fsOpenDir", 1, fsOpenDir);
    register_procedure("fsCloseDir", 1, fsCloseDir);
    register_procedure("fsReadDir", 1, fsReadDir);
    register_procedure("fsOpen", 2, fsOpen);
    register_procedure("fsClose", 1, fsClose);
    register_procedure("fsRead", 3, fsRead);
    register_procedure("fsWrite", 3, fsWrite);
    register_procedure("fsRemove", 1, fsRemove);

#ifdef _DEBUG_1_
    printRegisteredProcedures();
#endif

    launch_server();
    return 0;
}
