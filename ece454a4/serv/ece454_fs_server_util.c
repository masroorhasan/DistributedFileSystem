#include "ece454_fs_server.h"

/*
 * Creates a directory stream using the folder name
 * passed in on server launch.
 */
extern void setHostFolder(char* folder_name) {
    hosted_folder_name = folder_name;
}

extern void initDirEntries() {
		next_dir_entry = 0;

		int i = 0;
		for (; i < 256; i++) {
				dir_entries[i] = (DIR *)malloc(SIZE_DIR);
		}
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
 * operate on it.*/
/*
extern FSDIR* deserializeFSDIR(const int nparams, arg_type *a) {
    printf("deserializeFSDIR() helper method not implemented.");
    exit(0);

    // int size = sizeof(FSDIR);
		int size = 0;
    FSDIR *dir = (FSDIR *) malloc(size);
    memcpy(dir, (FSDIR *)a->arg_val, size);

    return dir;
}
*/

/*
 * Serializes FsDirent struct members from server so a client can
 * operate on it.
 */
extern return_type serializeFsDirent(struct dirent *d) {
    printf("serializeFsDirent() helper method not implemented.");
    exit(0);

    int entType = -1;
    if(d->d_type == DT_DIR) {
        entType = 1;
    } else if(d->d_type == DT_REG) {
        entType = 0;
    } else {
        entType = 255;
    }

    int sz = sizeof(int) + 256;
    char *buffer = (char *) malloc(sz);

    memcpy(buffer, &(entType), sizeof(int));
    memcpy(buffer + sizeof(int), &(d->d_name), 256);

    return_type ret;
    ret.return_size = sz;
    ret.return_val = (void *)malloc(sz);
    ret.return_val = buffer;

    return ret;
}
