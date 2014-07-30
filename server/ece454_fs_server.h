#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Directory related
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <dirent.h>

// Network types
#include "ece454rpc_types.h"

// Maintains size of dir
#define SIZE_DIR 200
#define MAX_DIR_SIZE 256

// Stores the name of directory hosted by the server
char *hosted_folder_name;

// Used to store open DIRs
DIR *dir_entries[MAX_DIR_SIZE];

// Used to keep track of next free index in dir_entries array
int next_dir_entry;

// Waiting state for concurrency protocol
typedef enum {ACK, NACK } waiting_state;

// Queue struct
typedef struct waitingList {
	const char* filepath;
	int uid;

	struct waitingList *next;
} waiting_list;

waiting_list *wl_queue;
int waiting_id;

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
extern void initDirEntries();
// extern FSDIR* deserializeFSDIR(const int nparams, arg_type *a);
extern return_type serializeFsDirent(struct dirent *d);
extern char* parseFolderPath(const char* folderPath);
