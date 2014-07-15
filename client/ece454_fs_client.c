#include "ece454_fs_client.h"

/*
 * Checks whether there's a call is valid based on client's
 * mounted state.
 *
 * Returns false if no error.
 * Returns true if error and sets errno.
 */
extern bool mountError(bool expected) {
    if (mounted != expected) {
        printf("Error expected mounted = ");
        printf(expected ? "true.\n" : "false.\n");
        return true;
    }

    return false;
}

/*
 * Mounts a remote server folder locally.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsMount(const char *srvIpOrDomName, const unsigned int srvPort, const char *localFolderName) {

    // Check that we aren't mounted
    if (mountError(false)) return -1;

    // Persisting server name, port and local folder name
    int localfolderName_size = (strlen(localFolderName) + 1);
    localDirName = (char *) malloc(sizeof(char) * localfolderName_size);
    memcpy(localDirName, localFolderName, sizeof(char) * localfolderName_size);

    int destAddr_size = strlen(srvIpOrDomName) + 1;
    destAddr = (char *) malloc(sizeof(char) * destAddr_size);
    memcpy(destAddr, srvIpOrDomName, sizeof(char) * destAddr_size);

    destPort = srvPort;

    return_type ans;
    ans = make_remote_call(destAddr,
		       destPort,
		       "fsMount", 0);

    printf("Got response from fsMount RPC.\n");

    int size = ans.return_size;
    int value = *(int *)(ans.return_val);

    if (value == 0) {
        mounted = true;
        printf("Folder was successfully mounted.\n");
    } else {
        mounted = false;
        printf("Folder mount was unsuccessful.\n");
    }

    return value;
}

/*
 * Unmounts a remote server folder.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsUnmount(const char *localFolderName) {

    // Check that we're mounted
    if (mountError(true)) return -1;

    return_type ans;
    ans = make_remote_call(destAddr,
               destPort,
               "fsUnmount", 0);

    printf("Got response from fsUnmount RPC.\n");

    int size = ans.return_size;
    int value = *(int *)(ans.return_val);

    if (value == 0) {
        mounted = false;
        printf("Folder was successfully unmounted.\n");
    } else {
        printf("Folder unmounted was unsuccessful.\n");
    }

    return value;
}

/*
 * Opens a local folder that has been mounted previously.
 * This may or may not be a subfolder within a folder perviously mounted.
 *
 * Returns pointer to folder on success.
 * Returns NULL on failures and sets errno.
 */
extern FSDIR* fsOpenDir(const char *folderName) {

    // Check that we're mounted
    if (mountError(true)) return NULL;

    return_type ans;
    ans = make_remote_call(destAddr,
               destPort,
               "fsOpenDir", 1,
               strlen(folderName) + 1,
               folderName);

    printf("Got response from fsOpenDir RPC.\n");
    
    FSDIR *dir = deserializeFSDIR(ans);
    return dir;
}

/*
 * Counterpart to fsOpenDir().
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsCloseDir(FSDIR * folder) {
    // Check that we're mounted
    if (mountError(true)) return -1;

    return_type ans;
    ans = make_remote_call(destAddr,
               destPort,
               "fsCloseDir", 1,
               sizeof(FSDIR),
               folder);

    printf("Got response from fsCloseDir RPC.\n");
    int size = ans.return_size;
    int ret_val = *(int *)ans.return_val;

    return ret_val;
}

/*
 * Returns the next entry in the folder.
 * This is repeatedly called to browse the contents of a folder.
 *
 * Returns fsDirent pointer on success.
 * Returns NULL on failure and sets errno.
 * Returns NULL when end of folder reacherd.
 */
extern struct fsDirent *fsReadDir(FSDIR * folder) {
    // Check that we're mounted
    if (mountError(true)) return NULL;

    return_type ans;
    ans = make_remote_call(destAddr,
               destPort,
               "fsReadDir", 1,
               sizeof(FSDIR),
               folder);

    printf("Got response from fsReadDir RPC.\n");

    struct fsDirent *dent = deserializeFsDirent(ans);
    return dent;
}

/*
 * Opens a file in either read (0) or write (1) mode.
 * If file does not exists and the mode is write it is created.
 *
 * Returns a positive integer file descriptor on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsOpen(const char *fname, int mode) {
    // Check that we're mounted
    if (mountError(true)) return -1;
    return -1;
}


/*
 * The counterpart to open(). The argument file descriptor is no longer
 * valid after this call.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsClose(int fd) {
    // Check that we're mounted
    if (mountError(true)) return -1;
    return -1;
}

/*
 * Used to read up to count bytes from fd into buf.
 * fd should be the return from a call to fsOpen() in read-mode.
 * This call will not read past the end of the file.
 * Subsequent calls to fsRead() should return subsequent bytes in the file.
 *
 * Returns number of bytes read into buf on success.
 * Returns -1 on error and sets errno.
 */
extern int fsRead(int fd, void *buf, const unsigned int count) {
    // Check that we're mounted
    if (mountError(true)) return -1;
    return -1;
}

/*
 * Writes up to count bytes from buf into the file referred to by fd.
 * fs should be the return from a call to fsOpen() in write-mode.
 * If the file already exists the first call to fsWrite() should overwrite the file with the
 * contents of buf.
 * Subsequent calls to an fd appends to the file.
 *
 * Returns number of bytes written on success.
 * Returns -1 on error and sets errno.
 */
extern int fsWrite(int fd, const void *buf, const unsigned int count) {
    // Check that we're mounted
    if (mountError(true)) return -1;
    return -1;
}

/*
 * Carefully removes a file or folder from the server to ensure
 * no concurrency issues.
 *
 * Returns fd of the file removed on success.
 * Returns -1 on error and sets errno.
 */
extern int fsRemove(const char *name) {
    // Check that we're mounted
    if (mountError(true)) return -1;
    return -1;
}

