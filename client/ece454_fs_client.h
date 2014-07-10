#include <string.h>
#include <stdbool.h>

#include "ece454_fs.h"
#include "ece454rpc_types.h"

const char* destAddr;
const unsigned int destPort;

// Local folder name remote folder refered to 
const char* localFolderName;

// Keeps track if client has mounted FS
bool mounted = false;
