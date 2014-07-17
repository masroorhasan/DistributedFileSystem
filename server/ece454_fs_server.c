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
    printf("Request to open folder: %s\n", folder_path);

    /*
     * Folder parsing logic.
     */
    int i = 0;
    bool found_slash = false;
    char *fwdslash = "/";
    for(; i < strlen(folder_path); i++) {
        if(folder_path[i] == '/') {
            found_slash = true;
            break;  
        }
    }

    char *parsed_folder;
    if(found_slash == true) {
        parsed_folder = (char *) malloc(strlen(hosted_folder_name) + strlen(folder_path) - i + 1);
        memcpy(parsed_folder, hosted_folder_name, strlen(hosted_folder_name) + 1);
        strcat(parsed_folder, folder_path + i);
    } else {
        parsed_folder = (char *) malloc(strlen(hosted_folder_name) + 1);
        memcpy(parsed_folder, hosted_folder_name, strlen(hosted_folder_name) + 1);
    }
		/*
		 * End of folder pasrsing logic.
     */

    printf("Opening folder path %s\n", parsed_folder);

    DIR* hosted_dir = opendir(parsed_folder);
    parsed_folder = NULL;

    int openDirErrno = 0;
    if(hosted_dir == NULL) openDirErrno = errno;

    return_type fsdir_return;

    if(openDirErrno == 0) {
        dir_entries[next_dir_entry] = hosted_dir;
        
        fsdir_return.return_size =  sizeof(int) + sizeof(int);
        fsdir_return.return_val = (void *) malloc(fsdir_return.return_size);

        memcpy(fsdir_return.return_val, &openDirErrno, sizeof(int));
        memcpy(fsdir_return.return_val + sizeof(int), &next_dir_entry, sizeof(int));    

		// Directory entry has been taken, advance index
		next_dir_entry++;
    } else {
		printf("Server fsOpenDir() Error: %i\n", openDirErrno);
        fsdir_return.return_size = sizeof(int);
        fsdir_return.return_val = (void *) malloc(fsdir_return.return_size);

        memcpy(fsdir_return.return_val, &openDirErrno, sizeof(int));
    }
    
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

    int size = a->arg_size;
    int *dir = (int*) malloc(sizeof(int));

    memcpy(dir, (int*) a->arg_val, size); 
		int ret_int = -1;
    int closeDirErrno = EPERM;

		if (dir_entries[*dir] != NULL) {
				ret_int = closedir(dir_entries[*dir]);
        if(ret_int == 0) closeDirErrno = 0;
        dir_entries[*dir] = NULL;
		}
	
    return_type closedir_ret;

    closedir_ret.return_size = sizeof(int) + sizeof(int);
    closedir_ret.return_val = (void *) malloc(closedir_ret.return_size);

    memcpy(closedir_ret.return_val, &closeDirErrno, sizeof(int));
    memcpy(closedir_ret.return_val + sizeof(int), &ret_int, sizeof(int));

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

    int *read_dir = (int *) a->arg_val;
    unsigned char entType = 255;

    int readDirErrno = errno;
    
    return_type fsreaddir_ret;        
    struct dirent *d = NULL;

    fsreaddir_ret.return_size = sizeof(int) + sizeof(unsigned char) + 256;
    fsreaddir_ret.return_val = (void *) malloc(fsreaddir_ret.return_size);
        
    if (dir_entries[*read_dir] == NULL) {
        readDirErrno = ENOENT;

        memcpy(fsreaddir_ret.return_val, &readDirErrno, sizeof(int));
        memcpy(fsreaddir_ret.return_val + sizeof(int), &(entType), sizeof(unsigned char));

        return fsreaddir_ret;
    }
       
    d = readdir(dir_entries[*read_dir]);

    if(d == NULL) {
        if(readDirErrno != errno) {
            readDirErrno = errno;
        } else {
            readDirErrno = 0;
        }

        memcpy(fsreaddir_ret.return_val, &readDirErrno, sizeof(int));
        memcpy(fsreaddir_ret.return_val + sizeof(int), &(entType), sizeof(unsigned char));

        return fsreaddir_ret;
    } 

    // Successful readdir operation
    readDirErrno = 0;

    if(d->d_type == DT_DIR) {
        entType = 1;
    } else if(d->d_type == DT_REG) {
        entType = 0;
    }

    int index = 0;
    memcpy(fsreaddir_ret.return_val, &readDirErrno, sizeof(int));
    index += sizeof(int);

    memcpy(fsreaddir_ret.return_val + index, &entType, sizeof(unsigned char));
    index += sizeof(unsigned char);

    memcpy(fsreaddir_ret.return_val + index, &(d->d_name), 256);
    index += 256;

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

