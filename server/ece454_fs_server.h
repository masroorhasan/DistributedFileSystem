#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

// File type and struct definitions
#include "ServerFSHelper.h"


extern void setHostFolder(char* folder_name);
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

