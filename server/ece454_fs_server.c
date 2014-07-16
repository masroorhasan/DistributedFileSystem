#include "ece454_fs_server.h"

/* We allocate a global variable for returns. However,
 * r.return_val is allocated dynamically. We look to
 * the server_stub to deallocate after it returns the
 * response to the client. */
return_type r;

/*
 * Mounts a remote server folder locally.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern return_type fsMount(const int nparams, arg_type *a) {
    struct stat fileStat;
    int *ret_int = (int *) malloc(sizeof(int));
    *ret_int = stat(hosted_folder_name, &fileStat);

    printf("fsMount() called.\n");

    // printf("File Size: \t\t%lld bytes\n", fileStat.st_size);
    // printf("Number of Links: \t%d\n", fileStat.st_nlink);
    // printf("File inode: \t\t%llu\n", fileStat.st_ino);

    return_type mount_return;

    mount_return.return_size = sizeof(int);
    mount_return.return_val = (void *)(ret_int);

    return mount_return;
}

/*
 * Unmounts a remote server folder.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern return_type fsUnmount(const int nparams, arg_type *a) {
    int *ret_int = (int *) malloc(sizeof(int));
    *ret_int = 0;

    printf("fsUnmount() called.\n");

    return_type unmount_return;
    unmount_return.return_size = sizeof(int);
    unmount_return.return_val = (void *)ret_int;

    return unmount_return;
}

/*
 * Opens a local folder that has been mounted previously.
 * This may or may not be a subfolder within a folder perviously mounted.
 *
 * Returns pointer to folder on success.
 * Returns NULL on failures and sets errno.
 */
extern return_type fsOpenDir(const int nparams, arg_type *a) {
    printf("fsOpenDir() called.\n");

    int arg_sz = a->arg_size;
    char *folder_path = (char *) malloc(arg_sz);
    memcpy(folder_path, (char *)a->arg_val, arg_sz);
    printf("Request to open folder name: %s\n", folder_path);

    FSDIR* hosted_dir = (FSDIR*) malloc(sizeof(FSDIR));
    hosted_dir = opendir(folder_path);
    printf("Returning FSDIR* to client: %x\n", hosted_dir);

    return_type fsdir_return;
    fsdir_return.return_size = sizeof(FSDIR);
    fsdir_return.return_val = (void *)malloc(sizeof(FSDIR));
    memcpy(fsdir_return.return_val, hosted_dir, sizeof(FSDIR));

    return fsdir_return;
}

/*
 * Counterpart to fsOpenDir().
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern return_type fsCloseDir(const int nparams, arg_type *a) {
    printf("fsCloseDir() called.\n");

    // FSDIR *dir = deserializeFSDIR(nparams, a);
    FSDIR *dir = (FSDIR *)a->arg_val;

    int *ret_int = (int *) malloc(sizeof(int));
    printf("Here is the memory address of the dir being close: %x\n", dir);
    *ret_int = closedir(dir);

    return_type closedir_ret;
    closedir_ret.return_size = sizeof(int);
    closedir_ret.return_val = (void *)ret_int;

    return closedir_ret;
}

/*
 * Returns the next entry in the folder.
 * This is repeatedly called to browse the contents of a folder.
 *
 * Returns fsDirent pointer on success.
 * Returns NULL on failure and sets errno.
 * Returns NULL when end of folder reacherd.
 */
extern return_type fsReadDir(const int nparams, arg_type *a) {
    printf("fsReadDir() called.\n");

    // Note: Double check if explicit malloc and memcpy required,
    // use deserializeFSDIR() if thats the case
    FSDIR *read_dir = (FSDIR *) a->arg_val;

    int entType = -1;
    char entName[256];
    const int initErrno = errno;

    return_type fsreaddir_ret;

    if(read_dir != NULL) {
        printf("Reading directory entry.\n");
        printf("Dir memory address before read: %x\n", read_dir);
        struct dirent *d = readdir(read_dir);
        printf("Response contained d_int: %llu\n", d->d_ino);
        printf("Response contained d_name: %s\n", d->d_name);

        if(d == NULL) {
            printf("Error reading directory entry: %s \n", strerror(errno));
        } else {
            // return_type fsreaddir_ret = serializeFsDirent(d);
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
    } else {
        printf("Error reading directory stream.\n");
    }

    return fsreaddir_ret;
}

/*
 * Opens a file in either read (0) or write (1) mode.
 * If file does not exists and the mode is write it is created.
 *
 * Returns a positive integer file descriptor on success.
 * Returns -1 on failure and sets errno.
 */
extern return_type fsOpen(const int nparams, arg_type *a) {
    return_type r;
    return r;
}


/*
 * The counterpart to open(). The argument file descriptor is no longer
 * valid after this call.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern return_type fsClose(const int nparams, arg_type *a) {
    return_type r;
    return r;
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
extern return_type fsRead(const int nparams, arg_type *a) {
    return_type r;
    return r;
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
extern return_type fsWrite(const int nparams, arg_type *a) {
    return_type r;
    return r;
}

/*
 * Carefully removes a file or folder from the server to ensure
 * no concurrency issues.
 *
 * Returns fd of the file removed on success.
 * Returns -1 on error and sets errno.
 */
extern return_type fsRemove(const int nparams, arg_type *a) {
    return_type r;
    return r;
}

