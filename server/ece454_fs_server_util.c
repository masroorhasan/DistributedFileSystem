#include "ece454_fs_server.h"

/*
 * Creates a directory stream using the folder name
 * passed in on server launch.
 */
extern void setHostFolder(char* folder_name) {
    hosted_folder_name = folder_name;
    hosted_dir = opendir(folder_name);
}

/*
 * Removes local folder name prefix and returns the true
 * folder path.
 */
extern char* parseFolderPath(const char* folderPath) {
    char* servFolderPath;
    return servFolderPath;
}

/*
 * Deserializes an FSDIR from the client so a server can
 * operate on it.
 */
extern FSDIR* deserializeFSDIR(const int nparams, arg_type *a) {
    int size = sizeof(FSDIR);
    FSDIR *dir = (FSDIR *) malloc(size);
    memcpy(dir, (FSDIR *)a->arg_val, size);

    return dir;
}

/*
 * Serializes FsDirent struct members from server so a client can
 * operate on it.
 */
extern char* serializeFsDirent(int entType, char entName[]) {
	int idx = 0;
    int sz = sizeof(int) + 256;
    char *buffer = (char *) malloc(sz);

    // Serializing 4 bytes for entType
    memcpy(buffer, &(entType), sizeof(int));
    idx += sizeof(int);

    // Serializing 256 bytes for entType
    memcpy(buffer+idx, &(entName), sizeof(entName));

    return buffer;
}

