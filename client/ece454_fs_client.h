#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ece454_fs.h"
#include "ece454rpc_types.h"

char* destAddr;
unsigned int destPort;

/*
 * Local folder name remote folder refered to
 */ 
char* localDirName;

/*
 * Keeps track if client has mounted FS
 */
bool mounted;

/*
 * fsdir list mapped with localdirname 
 */
typedef struct fsdirList {
	char *localdirname;
	FSDIR *remotefsdir;
} fsdir_list_type;

int opened_fsdir_index = 0;
fsdir_list_type fsdir_list[256];

/*
 * fd list mapped with localdirname
 */
typedef struct fdList {
	char *localdirname;
	int remotefd;
} fd_list_type;

int opened_fd_index = 0;
fd_list_type fd_list[256];

/*
 * Need multiple mounts support
 */
typedef struct mountedList {
    char *server_ip;
    unsigned int port;

    char *localDirName;
    bool mounted;

    struct mountedList *next;
} mounted_list_type;

int mounted_index = 0;
mounted_list_type mounted_list[256];

/*
 * Need consistancy protocol for file operation
 */
typedef enum {ACK, NACK } waiting_state;


extern bool mountError(bool expected);

// Utility methods
extern FSDIR* deserializeFSDIR(return_type ans);
extern struct fsDirent* deserializeFsDirent(return_type ans);
extern void printBuf(char *buf, int size);
