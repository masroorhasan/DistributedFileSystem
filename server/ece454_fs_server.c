/**
 * References:
 *
 * http://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html
 * http://stackoverflow.com/questions/9778806/serializing-a-class-with-a-pointer-in-c
 * http://stackoverflow.com/questions/504810/how-do-i-find-the-current-machines-full-hostname-in-c-hostname-and-domain-info
 *
 * Coding Style:
 *
 * http://www.cs.swarthmore.edu/~newhall/unixhelp/c_codestyle.html
 */

#include "ece454_fs_server.h"

/* We allocate a global variable for returns. However,
 * r.return_val is allocated dynamically. We look to
 * the server_stub to deallocate after it returns the
 * response to the client. */
return_type r;

/*
 * Utility function to view contents of Waiting List
 */
void printList() {
    printf("Printing List...\n");
    waiting_list *list = wl_queue;
    while(list != NULL) {
        printf("filepath %s, uid %i, next %p\n", list->filepath, list->uid, list->next);
        list = list->next;
    }
}

/*
 * Adds client to Waiting List Queue to maintain entry 
 * level consistancy
 *
 * Returns unique id of newly added client
 */
int addToWaitingQueue(const char* remotepath) {
    int uid = waiting_id;

    struct waitingList *node = (struct waitingList *) malloc(sizeof(struct waitingList));
    node->filepath = remotepath;
    node->uid = uid;
    node->next = NULL;

    if(wl_queue == NULL) {
        wl_queue = node;
    } else {
        waiting_list *list = wl_queue;
        while(list->next != NULL) {
            list = list->next;            
        }

        list->next = node;
    }

    waiting_id += 1;
    return uid;
}

/*
 * Search Waiting List for locked file
 *
 * Returns unique id of first client waiting in line for file
 * Returns -1 otherwise
 */
int searchWaitingList(const char* remotepath) {

    printf("Searching Waiting List...\n");
    if(wl_queue != NULL) {
        waiting_list *list = wl_queue;
        while(list != NULL) {
            if(strcmp(remotepath, list->filepath) == 0) {
                int id = list->uid;
                if(id != -1) return id;
            }
            
            list = list->next;            
        }
    }

    return -1;
}

/*
 * Remove client from Waiting List
 */
void removeFromWaitingList(const char* remotepath, int clientuid) {
    if(wl_queue != NULL) {
        waiting_list *list = wl_queue;
        while(list != NULL) {
            if(strcmp(remotepath, list->filepath) == 0) {
                if(clientuid == list->uid) {
                    list->uid = -1;
                    return;
                }
            }
            
            list = list->next;            
        }
    }
}

/*
 * Mounts a remote server folder locally.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern return_type fsMount(const int nparams, arg_type *a) {
    printf("fsMount() called.\n");

    struct stat fileStat;
    int *mountErrno = (int *) malloc(sizeof(int));
    int *ret_int = (int *) malloc(sizeof(int));
    *ret_int = stat(hosted_folder_name, &fileStat);

    if(*ret_int < 0) {
        *mountErrno = errno;
    }

    // Parse data payload to send back to client
    return_type mount_return;
    mount_return.return_size = sizeof(int) + sizeof(int);

    mount_return.return_val = (void *) malloc(mount_return.return_size);
    memcpy(mount_return.return_val, ret_int, sizeof(int));
    memcpy(mount_return.return_val + sizeof(int), mountErrno, sizeof(int));


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

    // Parse data payload to send back to client
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

    /*
     * Folder path parsing logic.
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
     * End of folder path pasrsing logic.
     */

    DIR* hosted_dir = opendir(parsed_folder);
    parsed_folder = NULL;

    int openDirErrno = 0;
    if(hosted_dir == NULL) openDirErrno = errno;

    // Parse data payload to send back to client
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

    // Parse data payload to send back to client
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

    // Check index of dir entry list to find valid remote dir*
    // Set errno on failure, parse payload and send back to client
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

    // Hack '.' and '..' case to be entType 1
    const char* dot = ".";
    const char* dotdot = "..";
    if ((strcmp(dot, ((char*)&(d->d_name))) == 0) || (strcmp(dotdot, ((char*)&d->d_name)) == 0)) {
        entType = 1;
    }

    // Parse payload and send back to client
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
    printf("fsOpen() called.\n");

    int fname_sz = a->arg_size;

    // Get filename to open
    char *fname = (char *) malloc(fname_sz);
    memcpy(fname, (char *)a->arg_val, fname_sz);

    // Parse file name with server path
    int i = 0;
    char *parsed_folder;

    char *fwdslash = "/";
    for(; i < strlen(fname); i++) {
        if(fname[i] == '/') {
            parsed_folder = (char *) malloc(strlen(hosted_folder_name) + strlen(fname) - i + 1);
            memcpy(parsed_folder, hosted_folder_name, strlen(hosted_folder_name) + 1);
            strcat(parsed_folder, fname + i);
            break;
        }
    }

    // Get file mode
    arg_type *nextarg = a->next;
    int mode_sz = nextarg->arg_size;

    int *mode = (int *) malloc(mode_sz);
    memcpy(mode, (int *)nextarg->arg_val, mode_sz);

    // Get requesting clientuid
    arg_type *uidarg = nextarg->next;
    int uid_sz = uidarg->arg_size;

    int *id = (int *) malloc(uid_sz);
    memcpy(id, (int *)uidarg->arg_val, uid_sz);
    int clientuid = *id;

    int flags = -1;
    int openErrno = 0;

    if(*mode == 0) {
        flags = O_RDONLY;
    } else if(*mode == 1) {
        flags = O_WRONLY | O_TRUNC | O_CREAT;
    }

    return_type fsopen_ret;
    /*
     * Check waiting list queue for first uid with filename == parsed_folder
     */
    int founduid = searchWaitingList(parsed_folder);
    printList();

    if(founduid == -1) {
        // Couldnt find file in waiting list
        int open_fd = open(parsed_folder, flags, S_IRWXU);
        
        // open failed with bad filename
        if(open_fd == -1) {
            openErrno = errno;
            printf("fsOpen Error: %s\n", strerror(openErrno));

            // Open failed, parse payload with state, uid, errno and fd
            int state = ACK;
            int uid = -1;
            fsopen_ret.return_size = (sizeof(int)) * 4;
            fsopen_ret.return_val = (void *) malloc(fsopen_ret.return_size);

            memcpy(fsopen_ret.return_val, &state, sizeof(int));
            memcpy(fsopen_ret.return_val + sizeof(int), &uid, sizeof(int));
            memcpy(fsopen_ret.return_val + (sizeof(int)*2), &openErrno, sizeof(int));
            memcpy(fsopen_ret.return_val + (sizeof(int)*3), &open_fd, sizeof(int));

            return fsopen_ret;
        }

        int filelock = flock(open_fd, LOCK_EX | LOCK_NB);

        // Attempt at getting file lock
        if(filelock == -1) {
            // Couldn't get lock, add client to waiting list
            int uidadded = addToWaitingQueue(parsed_folder);
            
            // Parse payload with state, uid
            int state = NACK;

            fsopen_ret.return_size = sizeof(int) * 2;
            fsopen_ret.return_val = (void *) malloc(fsopen_ret.return_size);
            memcpy(fsopen_ret.return_val, &state, sizeof(int));
            memcpy(fsopen_ret.return_val + sizeof(int), &uidadded, sizeof(int));

            return fsopen_ret;
        }

        // Got lock, parse payload with state, uid, errno and fd
        int state = ACK;
        int uid = -1;
        fsopen_ret.return_size = (sizeof(int)) * 4;
        fsopen_ret.return_val = (void *) malloc(fsopen_ret.return_size);

        memcpy(fsopen_ret.return_val, &state, sizeof(int));
        memcpy(fsopen_ret.return_val + sizeof(int), &uid, sizeof(int));
        memcpy(fsopen_ret.return_val + (sizeof(int)*2), &openErrno, sizeof(int));
        memcpy(fsopen_ret.return_val + (sizeof(int)*3), &open_fd, sizeof(int));

        return fsopen_ret;
    } else {
        if(founduid == clientuid) {
            // Found requesting client at head of Waiting List for this file
            int open_fd = open(parsed_folder, flags, S_IRWXU);
            
            // open failed with bad filename
            if(open_fd == -1) {
                openErrno = errno;
                printf("fsOpen Error: %s\n", strerror(openErrno));

                // Open failed, parse payload with state, uid, errno and fd
                int state = ACK;
                int uid = -1;
                fsopen_ret.return_size = (sizeof(int)) * 4;
                fsopen_ret.return_val = (void *) malloc(fsopen_ret.return_size);

                memcpy(fsopen_ret.return_val, &state, sizeof(int));
                memcpy(fsopen_ret.return_val + sizeof(int), &uid, sizeof(int));
                memcpy(fsopen_ret.return_val + (sizeof(int)*2), &openErrno, sizeof(int));
                memcpy(fsopen_ret.return_val + (sizeof(int)*3), &open_fd, sizeof(int));

                return fsopen_ret;
            }

            int filelock = flock(open_fd, LOCK_EX | LOCK_NB);
            
            // File is still locked on attempt
            if(filelock == -1) {
                int state = NACK;

                // Parse payload with NACK state, uid, errno and fd
                fsopen_ret.return_size = sizeof(int) * 2;
                fsopen_ret.return_val = (void *) malloc(fsopen_ret.return_size);
                memcpy(fsopen_ret.return_val, &state, sizeof(int));
                memcpy(fsopen_ret.return_val + sizeof(int), &clientuid, sizeof(int));

                return fsopen_ret;
            }

            // Got lock for file, remove client from Waiting List
            removeFromWaitingList(parsed_folder, clientuid);

            // Parse payload with state, uid, errno and fd
            int state = ACK;
            int uid = -1;
            fsopen_ret.return_size = (sizeof(int)) * 4;
            fsopen_ret.return_val = (void *) malloc(fsopen_ret.return_size);

            memcpy(fsopen_ret.return_val, &state, sizeof(int));
            memcpy(fsopen_ret.return_val + sizeof(int), &uid, sizeof(int));
            memcpy(fsopen_ret.return_val + (sizeof(int)*2), &openErrno, sizeof(int));
            memcpy(fsopen_ret.return_val + (sizeof(int)*3), &open_fd, sizeof(int));

            return fsopen_ret;

        }

        // New client, append to Waiting List after the clients ahead in line
        if(clientuid == -1) {
            int uidadded = addToWaitingQueue(parsed_folder);
            
            // Parse payload with state, uid
            int state = NACK;

            fsopen_ret.return_size = sizeof(int) * 2;
            fsopen_ret.return_val = (void *) malloc(fsopen_ret.return_size);
            memcpy(fsopen_ret.return_val, &state, sizeof(int));
            memcpy(fsopen_ret.return_val + sizeof(int), &uidadded, sizeof(int));

            return fsopen_ret;
        }

        // Client requesting file not at head of Waiting List
        int state = NACK;

        fsopen_ret.return_size = sizeof(int) * 2;
        fsopen_ret.return_val = (void *) malloc(fsopen_ret.return_size);
        memcpy(fsopen_ret.return_val, &state, sizeof(int));
        memcpy(fsopen_ret.return_val + sizeof(int), &clientuid, sizeof(int));

        return fsopen_ret;
    }
}


/*
 * The counterpart to open(). The argument file descriptor is no longer
 * valid after this call.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern return_type fsClose(const int nparams, arg_type *a) {
    printf("fsClose() called.\n");

    int fd_sz = a->arg_size;

    int closeErrno = 0;

    int fd;
    memcpy(&fd, (int *)a->arg_val, fd_sz);

    // Closing and unlocking file
    int close_fd = close(fd);
    if(close_fd == -1) {
        closeErrno = errno;
        printf("fsClose() %s\n", strerror(errno));
    }

    printf("Closing and unlocking file\n");
    int fileunlock = flock(fd, LOCK_UN | LOCK_NB);

    // Parse payload and sending back to client
    return_type fsclose_ret;    
    fsclose_ret.return_size = sizeof(int) + sizeof(int);
    fsclose_ret.return_val = (void *) malloc(fsclose_ret.return_size);

    memcpy(fsclose_ret.return_val, &closeErrno, sizeof(int));
    memcpy(fsclose_ret.return_val + sizeof(int), &close_fd, sizeof(int));

    return fsclose_ret;
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
    printf("fsRead() called.\n");

    // Get fd
    int fd_sz = a->arg_size;
    int fd;
    memcpy(&fd, (int *)a->arg_val, fd_sz);

    // Get buffer
    arg_type *buffarg = a->next;
    int buf_sz = buffarg->arg_size;
    char *buff = (char *) malloc(buf_sz);
    memcpy(buff, (char *)buffarg->arg_val, buf_sz);

    // Get count value
    arg_type *countarg = buffarg->next;
    int count_sz = countarg->arg_size;
    unsigned int count;
    memcpy(&count, (unsigned int *)countarg->arg_val, count_sz);

    int readErrno = 0;
    int bytes = read(fd, (void *)buff, count);

    // Parse payload and sending back to client
    return_type fsread_ret;
    fsread_ret.return_size = sizeof(int) + sizeof(int) + count;
    fsread_ret.return_val = (void *) malloc(fsread_ret.return_size);

    if (bytes == -1) {
        readErrno = errno;
        printf("fsRead() Error: %s\n", strerror(readErrno));
    }

    memcpy(fsread_ret.return_val, &readErrno, sizeof(int));
    memcpy(fsread_ret.return_val + sizeof(int), &bytes, sizeof(int));
    memcpy(fsread_ret.return_val + sizeof(int) + sizeof(int), buff, count);

    return fsread_ret;
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
    printf("fsWrite() called.\n");

    // Get fd
    int fd_sz = a->arg_size;
    int fd;
    memcpy(&fd, (int *)a->arg_val, fd_sz);

    // Get buffer
    arg_type *buffarg = a->next;
    int buf_sz = buffarg->arg_size;
    char *buff = (char *) malloc(buf_sz);
    memcpy(buff, (char *)buffarg->arg_val, buf_sz);

    // Get count value
    arg_type *countarg = buffarg->next;
    int count_sz = countarg->arg_size;
    unsigned int count;
    memcpy(&count, (unsigned int *)countarg->arg_val, count_sz);

    int writeErrno = 0;
    int bytes = write(fd, (void *)buff, (size_t)count);

    // Parse payload and send back to client

    if(bytes == -1) {
        writeErrno = errno;
        printf("fsWrite on server: %s\n", strerror(writeErrno));
    }

    return_type fswrite_ret;
    fswrite_ret.return_size = sizeof(int) + sizeof(int);
    fswrite_ret.return_val = (void *) malloc(fswrite_ret.return_size);

    memcpy(fswrite_ret.return_val, &writeErrno, sizeof(int));
    memcpy(fswrite_ret.return_val + sizeof(int), &bytes, sizeof(int));

    return fswrite_ret;
}

/*
 * Carefully removes a file or folder from the server to ensure
 * no concurrency issues.
 *
 * Returns fd of the file removed on success.
 * Returns -1 on error and sets errno.
 */
extern return_type fsRemove(const int nparams, arg_type *a) {
    printf("fsRemove() called\n");

    int name_sz = a->arg_size;
    char *name = (char *) malloc(name_sz);
    memcpy(name, (char *)a->arg_val, name_sz);

    /*
     * Folder parsing logic.
     */
    int i = 0;
    bool found_slash = false;
    char *fwdslash = "/";
    for(; i < strlen(name); i++) {
        if(name[i] == '/') {
            found_slash = true;
            break;
        }
    }

    char *parsed_folder;
    if(found_slash == true) {
        parsed_folder = (char *) malloc(strlen(hosted_folder_name) + strlen(name) - i + 1);
        memcpy(parsed_folder, hosted_folder_name, strlen(hosted_folder_name) + 1);
        strcat(parsed_folder, name + i);
    } else {
        parsed_folder = (char *) malloc(strlen(hosted_folder_name) + 1);
        memcpy(parsed_folder, hosted_folder_name, strlen(hosted_folder_name) + 1);
    }
    /*
     * End of folder pasrsing logic.
     */
    
    // Get uid from client
    arg_type *uidarg = a->next;
    int uid_sz = uidarg->arg_size;
    int *id = (int *) malloc(uid_sz);
    memcpy(id, (int *)uidarg->arg_val, uid_sz);
    int clientuid = *id;

    return_type fsremove_ret;

    // Search Waiting List to see who is ahead in line to get this file
    int founduid = searchWaitingList(parsed_folder);
    printList();

    // Couldnt find file in waiting list
    if(founduid == -1) {
        // Open file to get fd and attempt to get lock
        int rm_fd = open(parsed_folder, O_RDONLY);

        // File does not exist, parse payload and send back to client
        if(rm_fd == -1) {
            printf("Error: %s\n", strerror(errno));

            int removeErrno = errno;

            fsremove_ret.return_size = sizeof(int) * 4;
            fsremove_ret.return_val = malloc(fsremove_ret.return_size);

            int state = ACK;
            int uid = -1; 
            int remove_ret = -1;

            memcpy(fsremove_ret.return_val, &state, sizeof(int));
            memcpy(fsremove_ret.return_val + sizeof(int), &uid, sizeof(int));
            memcpy(fsremove_ret.return_val + (sizeof(int)*2), &removeErrno, sizeof(int));
            memcpy(fsremove_ret.return_val + (sizeof(int)*3), &remove_ret, sizeof(int));

            return fsremove_ret;
        }

        int rm_lock = flock(rm_fd, LOCK_EX | LOCK_NB);
        if(rm_lock == -1) {
            // Lock already taken for file, add to Waiting List
            // Parse payload and send back to client
            int uidadded = addToWaitingQueue(parsed_folder);

            int state = NACK;
            
            fsremove_ret.return_size = sizeof(int) * 2;
            fsremove_ret.return_val = malloc(fsremove_ret.return_size);

            memcpy(fsremove_ret.return_val, &state, sizeof(int));
            memcpy(fsremove_ret.return_val + sizeof(int), &uidadded, sizeof(int));

            return fsremove_ret;   
        }

        // Got lock, remove file, unlock
        int removeErrno = 0;
        int remove_ret = remove(parsed_folder);
        if(remove_ret == -1) {
            removeErrno = errno;
            printf("fsRemove() Error: %s\n", strerror(removeErrno));
        }

        int fileunlock = flock(rm_fd, LOCK_UN | LOCK_NB);

        // Parse payload with ACK, uid, errno and rm_ret
        int state = ACK;
        int uid = -1;

        fsremove_ret.return_size = sizeof(int) * 4;
        fsremove_ret.return_val = malloc(fsremove_ret.return_size);

        memcpy(fsremove_ret.return_val, &state, sizeof(int));
        memcpy(fsremove_ret.return_val + sizeof(int), &uid, sizeof(int));
        memcpy(fsremove_ret.return_val + (sizeof(int)*2), &removeErrno, sizeof(int));
        memcpy(fsremove_ret.return_val + (sizeof(int)*3), &remove_ret, sizeof(int));

        return fsremove_ret;
    } else {
        if(founduid == clientuid) {
            // Found requesting client at head of Waiting List for this file

            // Attempt to get lock for file
            int rm_fd = open(parsed_folder, O_RDONLY);

            // File does not exist, parse payload and send back to client
            if(rm_fd == -1) {
                printf("Error: %s\n", strerror(errno));

                int removeErrno = errno;

                fsremove_ret.return_size = sizeof(int) * 4;
                fsremove_ret.return_val = malloc(fsremove_ret.return_size);

                int state = ACK;
                int uid = -1; 
                int remove_ret = -1;

                memcpy(fsremove_ret.return_val, &state, sizeof(int));
                memcpy(fsremove_ret.return_val + sizeof(int), &uid, sizeof(int));
                memcpy(fsremove_ret.return_val + (sizeof(int)*2), &removeErrno, sizeof(int));
                memcpy(fsremove_ret.return_val + (sizeof(int)*3), &remove_ret, sizeof(int));

                return fsremove_ret;
            }

            int rm_lock = flock(rm_fd, LOCK_EX | LOCK_NB);
            if(rm_lock == -1) {
                // Lock already taken for file, add to Waiting List
                if(clientuid == -1) {
                    int uidadded = addToWaitingQueue(parsed_folder);

                    int state = NACK;
                    
                    fsremove_ret.return_size = sizeof(int) * 2;
                    fsremove_ret.return_val = malloc(fsremove_ret.return_size);

                    memcpy(fsremove_ret.return_val, &state, sizeof(int));
                    memcpy(fsremove_ret.return_val + sizeof(int), &uidadded, sizeof(int));

                    return fsremove_ret;    
                }

                int state = NACK;

                fsremove_ret.return_size = sizeof(int) * 2;
                fsremove_ret.return_val = (void *) malloc(fsremove_ret.return_size);
                memcpy(fsremove_ret.return_val, &state, sizeof(int));
                memcpy(fsremove_ret.return_val + sizeof(int), &clientuid, sizeof(int));

                return fsremove_ret; 
            }

            
            int removeErrno = 0;
            int remove_ret = remove(parsed_folder);
            if(remove_ret == -1) {
                removeErrno = errno;
                printf("fsRemove() Error: %s\n", strerror(removeErrno));
            }

            // Got lock, remove from waiting list
            removeFromWaitingList(parsed_folder, clientuid);

            int fileunlock = flock(rm_fd, LOCK_UN | LOCK_NB);

            // parse payload with ACK, uid, errno and rm_ret
            int state = ACK;
            int uid = -1;

            fsremove_ret.return_size = sizeof(int) * 4;
            fsremove_ret.return_val = malloc(fsremove_ret.return_size);

            memcpy(fsremove_ret.return_val, &state, sizeof(int));
            memcpy(fsremove_ret.return_val + sizeof(int), &uid, sizeof(int));
            memcpy(fsremove_ret.return_val + (sizeof(int)*2), &removeErrno, sizeof(int));
            memcpy(fsremove_ret.return_val + (sizeof(int)*3), &remove_ret, sizeof(int));

            return fsremove_ret;

        }

        if(clientuid == -1) {
            // New client, so add to Waiting List
            int uidadded = addToWaitingQueue(parsed_folder);

            int state = NACK;

            fsremove_ret.return_size = sizeof(int) * 2;
            fsremove_ret.return_val = (void *) malloc(fsremove_ret.return_size);
            memcpy(fsremove_ret.return_val, &state, sizeof(int));
            memcpy(fsremove_ret.return_val + sizeof(int), &uidadded, sizeof(int));

            return fsremove_ret;
        }

        // Client requesting file not at head of Waiting List
        int state = NACK;

        fsremove_ret.return_size = sizeof(int) * 2;
        fsremove_ret.return_val = (void *) malloc(fsremove_ret.return_size);
        memcpy(fsremove_ret.return_val, &state, sizeof(int));
        memcpy(fsremove_ret.return_val + sizeof(int), &clientuid, sizeof(int));

        return fsremove_ret;        
    }
}

