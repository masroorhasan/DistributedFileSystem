#include "ece454_fs_client.h"

/*
 * Deserializes an FSDIR from the client so a server can
 * operate on it.
 */
extern FSDIR* deserializeFSDIR(return_type ans) {
    int size = ans.return_size;
    FSDIR *dir = (FSDIR *) malloc(size);
    memcpy(dir, (FSDIR *)ans.return_val, size);

    return dir;
}

/*
 * Deserializes return_type from server to package the 
 * individual members into fsDirent struct
 */
extern struct fsDirent* deserializeFsDirent(return_type ans) {
    printf("deserializeFSDirent called.\n");
	  struct fsDirent *fdent = (struct fsDirent *) malloc(sizeof(struct fsDirent));
    printf("Memory allocated for fsDirent.\n");

    int entType;
    memcpy(&entType, (int *)ans.return_val, sizeof(int));

    printf("Deserializing: %d\n", entType);

    char *entName = (char *) malloc(256);
    memcpy(entName, (char *)ans.return_val + sizeof(int), 256);

    printf("Deserializing: %s\n", entName);
    fdent->entType = entType;
    strncpy(fdent->entName, entName, 256);

    return fdent;
}
