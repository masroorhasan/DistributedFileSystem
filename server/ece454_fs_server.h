#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// Directory related
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <string.h>

// Network types
#include "ece454rpc_types.h"

/*
 * Allows you to create variables of type FSDIR
 * which is just a wrapper around DIR for the time being.
 *
 * TODO: Design and implement this type.
 */
typedef DIR FSDIR;

/*
 * Given, do not modify on server or client.
 */
struct fsDirent {
    char entName[256];
    unsigned char entType; /* 0 for file, 1 for folder,
			      -1 otherwise. */
};

// Stores the directory hosted by the server
FSDIR* hosted_dir;

// Stores the name of directory hosted by the server
char* hosted_folder_name;

extern return_type fsMount(const int nparams, arg_type *a);
extern return_type fsUnmount(const int nparams, arg_type *a);
extern return_type fsOpenDir(const int nparams, arg_type *a);
extern return_type fsCloseDir(const int nparams, arg_type *a);
extern return_type fsReadDir(const int nparams, arg_type *a);
extern return_type fsOpen(const int nparams, arg_type *a);
extern return_type fsClose(const int nparams, arg_type *a);
extern return_type fsRead(const int nparams, arg_type *a);
extern return_type fsWrite(const int nparams, arg_type *a);
extern return_type fsRemove(const int nparams, arg_type *a);

// Utility methods
extern void setHostFolder(char* folder_name);
extern return_type serializeFSDIR(FSDIR* folder);
extern FSDIR* deserializeFSDIR(const int nparams, arg_type *a);
extern char* parseFolderPath(const char* folderPath);
