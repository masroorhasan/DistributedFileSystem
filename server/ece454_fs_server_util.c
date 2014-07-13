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
 * Serilaizes an FSDIR to return_type so it can be sent
 * back to a client.
 */
extern return_type serializeFSDIR(FSDIR* folder) {
    return_type r;
    return r;
}

/*
 * Deserializes an FSDIR from the client so a server can
 * operate on it.
 */
extern FSDIR* deserializeFSDIR(const int nparams, arg_type *a) {
    FSDIR* fd;
    return fd;
}

