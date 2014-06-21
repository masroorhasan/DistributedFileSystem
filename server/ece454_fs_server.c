#include <stdio.h>
#include "ece454_fs.h"

/*
 * Mounts a remote server folder locally.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsMount(const char *srvIpOrDomName, const unsigned int srvPort, const char *localFolderName) {
    return -1;
}

/*
 * Unmounts a remote server folder.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsUnmount(const char *localFolderName) {
    return -1;
}

/*
 * Opens a local folder that has been mounted previously.
 * This may or may not be a subfolder within a folder perviously mounted.
 *
 * Returns pointer to folder on success.
 * Returns NULL on failures and sets errno.
 */
extern FSDIR* fsOpenDir(const char *folderName) {
    FSDIR *d;
    return d;
}

/*
 * Counterpart to fsOpenDir().
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsCloseDir(FSDIR * folder) {
    return -1;
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
    struct fsDirent *d;
    return d;
}

/*
 * Opens a file in either read (0) or write (1) mode.
 * If file does not exists and the mode is write it is created.
 *
 * Returns a positive integer file descriptor on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsOpen(const char *fname, int mode) {
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
    return -1;
}

