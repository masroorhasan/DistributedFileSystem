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

    FSDIR* hosted_dir = opendir(folder_path);

    return_type fsdir_return;
    fsdir_return.return_size = sizeof(FSDIR);
    fsdir_return.return_val = (void *)(hosted_dir);

    return fsdir_return;
}

/*
 * Counterpart to fsOpenDir().
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern return_type fsCloseDir(const int nparams, arg_type *a) {
    return_type closedir_ret;

    int size = sizeof(FSDIR);
    FSDIR *dir = (FSDIR *) malloc(size);
    memcpy(dir, (FSDIR *)a->arg_val, size);

    int *ret_int = (int *) malloc(sizeof(int));
    *(ret_int) = closedir(dir);

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

    int size = sizeof(FSDIR);
    FSDIR *read_dir = (FSDIR *) malloc(size);
    memcpy(read_dir, (FSDIR *)a->arg_val, size);

    // struct fsDirent dent;
    int entType = -1;
    char entName[256];
    const int initErrno = errno;

    return_type fsreaddir_ret;

    if(read_dir != NULL) {
        printf("Printing directory contents.\n");
        struct dirent *d = readdir(read_dir);

        if(d == NULL) {
            // if(errno == initErrno) errno = 0;
            printf("error: %s \n", strerror(errno));
        } else {
            if(d->d_type == DT_DIR) {
                entType = 1;
            }
            else if(d->d_type == DT_REG) {
                entType = 0;
            }
            else {
                entType = -1;
            }

            memcpy(&(entName), &(d->d_name), 256);
            printf("entityType: %i, entityName: %s\n", entType, entName);
            
            // Serialize entType and entName
            int idx = 0;
            int sz = sizeof(int) + 256;
            char *buffer = (char *) malloc(sz);

            memcpy(buffer, &(entType), sizeof(int));
            idx += sizeof(int);

            memcpy(buffer+idx, &(entName), sizeof(entName));
            // idx += sizeof(entName);

            fsreaddir_ret.return_size = sz;
            fsreaddir_ret.return_val = (void *)buffer;

            return fsreaddir_ret;
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

