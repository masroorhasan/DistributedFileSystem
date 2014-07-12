#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ece454_fs.h"
#include "ece454rpc_types.h"

char* destAddr;
unsigned int destPort;

// Local folder name remote folder refered to
char* localDirName;

// Keeps track if client has mounted FS
bool mounted;

extern bool mountError(bool expected);

// Utility methods
extern return_type serializeFSDIR(FSDIR* folder);
extern FSDIR* deserializeFSDIR(const int nparams, arg_type *a);
