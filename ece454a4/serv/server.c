#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ece454_fs_server.h"

#if 0
#define _DEBUG_1_
#endif


extern void printRegisteredProcedures();


int main(int argc, char *argv[]) {

    char *hosted_folder = argv[1];
    setHostFolder(hosted_folder);
		initDirEntries(); 

    if (argc < 2) {
        printf("Error folder name to be hosted not provided.\n");
        return 0;
    }

    // Register file system procedures
    register_procedure("fsMount", 0, fsMount);
    register_procedure("fsUnmount", 0, fsUnmount);
    register_procedure("fsOpenDir", 1, fsOpenDir);
    register_procedure("fsCloseDir", 1, fsCloseDir);
    register_procedure("fsReadDir", 1, fsReadDir);
    register_procedure("fsOpen", 3, fsOpen);
    register_procedure("fsClose", 1, fsClose);
    register_procedure("fsRead", 3, fsRead);
    register_procedure("fsWrite", 3, fsWrite);
    register_procedure("fsRemove", 2, fsRemove);

#ifdef _DEBUG_1_
    printf("The folder to be hosted is: %s \n", hosted_folder);
    printRegisteredProcedures();
#endif

    launch_server();
    return 0;
}
