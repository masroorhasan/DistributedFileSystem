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
        errno = EPERM;
        printf("fsMount() Error: %s \n", strerror(errno));
        return true;
    }

    return false;
}

bool checkMountedState(const char *localdirname) {
    
    if(mounted_index == 0) return true;
    bool call_server = false;
    
    int i = 0;
    for(; i < mounted_index; i++) {
        if(strcmp(mounted_list[i].localDirName, localdirname) == 0) {
            if(mounted_list[i].mounted == false) {
                call_server = true;
                break;
            } else {
                call_server = false;
                break;
                printf("already mounted.\n");
            }
        } else {
            call_server = true;
        }
    }

    return call_server;
}

int checkUnmountedState(const char *localdirname) {
    if(mounted_index == 0) return -1;

    int i = 0;
    for(; i < mounted_index; i++) {
        if(strcmp(mounted_list[i].localDirName, localdirname) == 0) {
            if(mounted_list[i].mounted == true) {
                mounted_list[i].mounted = false;
                return i;
            }
        } 
    }

    return -1;
}

void populateMountList(const char *serverip, const unsigned int port, const char *localdirname) {
    mounted_list_type node; // = (struct mountedList *) malloc(sizeof(struct mountedList));
    node.server_ip = serverip;
    node.port = port;
    node.localDirName = localdirname;
    node.mounted = true;

    mounted_list[mounted_index] = node;
    mounted_index++;
}

void printList() {
    int i = 0;
    for(; i < mounted_index; i++) {
         printf("serverip %s, port %i, localdir %s, mounted %i\n", 
            mounted_list[i].server_ip, mounted_list[i].port, mounted_list[i].localDirName,
            mounted_list[i].mounted);
    }
}

/*
 * Mounts a remote server folder locally.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsMount(const char *srvIpOrDomName, const unsigned int srvPort, const char *localFolderName) {
    printf("fsMount()\n");

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
    ans = make_remote_call(
               destAddr,
		       destPort,
		       "fsMount", 0);

    printf("Got response from fsMount RPC.\n");

    int size = ans.return_size;
    int value;
    memcpy(&value, (int *)ans.return_val, sizeof(int));

    if (value == 0) {
        mounted = true;
        printf("Folder was successfully mounted.\n");
    } else {
        mounted = false;
        int mountErrno;
        memcpy(&mountErrno, (int *)(ans.return_val + sizeof(int)), sizeof(int));

        errno = mountErrno;
        printf("fsMount() Error: %s \n", strerror(errno));
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

    char *root_path = (char *) malloc(strlen(localDirName) + 1);
    memcpy(root_path, localFolderName, strlen(localDirName));

    if(strcmp(root_path, localDirName) != 0) {
        errno = EPERM;
        printf("fsUnmount() Error: %s \n", strerror(errno));
        return -1;
    }

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

    char *root_path = (char *) malloc(strlen(localDirName) + 1);
    memcpy(root_path, folderName, strlen(localDirName));

    if(strcmp(root_path, localDirName) != 0) {
        errno = ENOENT;
        return NULL;
    }

    return_type ans;
    ans = make_remote_call(destAddr,
               destPort,
               "fsOpenDir", 1,
               strlen(folderName) + 1,
               folderName);

    printf("Got response from fsOpenDir RPC.\n");

    int size = ans.return_size;

    int openDirErrno;
    memcpy(&openDirErrno, (int *)ans.return_val, sizeof(int));

    FSDIR *dir = (FSDIR *) malloc(sizeof(FSDIR));

    if(openDirErrno == 0) {
        memcpy(dir, (FSDIR *)(ans.return_val + sizeof(int)), sizeof(FSDIR));
    } else {
        dir = NULL;
        errno = openDirErrno;
        printf("fsDirOpen() Error: %s \n", strerror(errno));
    }

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

    int closeDirErrno;
    memcpy(&closeDirErrno, (int *)ans.return_val, sizeof(int));

	  int ret_val = -1;
	  if (closeDirErrno == 0) {
        memcpy(&ret_val, (int *)(ans.return_val + sizeof(int)), sizeof(int));
	  } else {
        errno = closeDirErrno;
        printf("fsCloseDir() Error: %s\n", strerror(errno));
	  }

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

    int index = 0;

    int readDirErrno;
    memcpy(&readDirErrno, (int *)ans.return_val, sizeof(int));
    index += sizeof(int);

    unsigned char entType;
    memcpy(&entType, (unsigned char *)(ans.return_val + index), sizeof(unsigned char));
    index += sizeof(unsigned char);

    if(readDirErrno == 0 && entType == 255) {
        printf("End of stream\n");
        return NULL;
    }

    if(readDirErrno != 0 && entType == 255) {
        errno = readDirErrno;
        printf("fsReadDir() Error: %s \n", strerror(errno));
        return NULL;
    }

    char *entName = (char *) malloc(256);
    memcpy(entName, (char *)(ans.return_val + index), 256);
    index += 256;

    struct fsDirent *fdent = (struct fsDirent *) malloc(sizeof(struct fsDirent));
    fdent->entType = entType;
    strncpy(fdent->entName, entName, 256);

    return fdent;
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

    char *root_path = (char *) malloc(strlen(localDirName) + 1);
    memcpy(root_path, fname, strlen(localDirName));
    printf("root_path %s\n", root_path);

    if(strcmp(root_path, localDirName) != 0) {
        errno = ENOENT;
        return -1;
    }

    int fname_sz = strlen(fname) + 1;
    int mode_sz = sizeof(int);

    int uid = -1;

    return_type ans;
    ans = make_remote_call(destAddr,
              destPort,
              "fsOpen", 3,
              fname_sz,
              fname,
              mode_sz,
              &mode,
              sizeof(int),
              &uid);

    printf("Got response from fsOpen RPC.\n");
    int sz = ans.return_size;

    // If NACK sent back from server, file was locked, keep trying    
    waiting_state state;
    memcpy(&state, (int *)ans.return_val, sizeof(int));
    printf("state: %i\n", state);

    memcpy(&uid, (int *)(ans.return_val + sizeof(int)), sizeof(int));
    printf("uid: %i\n", uid);

    while (state == NACK && uid != -1) {
        ans = make_remote_call(destAddr,
              destPort,
              "fsOpen", 3,
              fname_sz,
              fname,
              mode_sz,
              &mode,
              sizeof(int),
              &uid);

        memcpy(&state, (int *)ans.return_val, sizeof(int));
        printf("state: %i\n", state);

        memcpy(&uid, (int *)(ans.return_val + sizeof(int)), sizeof(int));
        printf("uid: %i\n", uid);

        printf("sleeping for 2s...\n");
        sleep(2);
    }

    int openErrno;
    memcpy(&openErrno, (int *)(ans.return_val + (sizeof(int)*2)), sizeof(int));

    if(openErrno != 0) {
        errno = openErrno;
        printf("fsOpen() Error: %s\n", strerror(errno));
    }

    int open_fd;
    memcpy(&open_fd, (int *)(ans.return_val + (sizeof(int)*3)), sizeof(int));

    return open_fd;
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

    return_type ans;
    ans = make_remote_call(destAddr,
              destPort,
              "fsClose", 1,
              sizeof(int),
              &fd);

    printf("Got response from fsClose RPC.\n");
    int sz = ans.return_size;

    int closeErrno;
    memcpy(&closeErrno, (int *)(ans.return_val), sizeof(int));

    if(closeErrno != 0) {
        errno = closeErrno;
        printf("fsClose() Error: %s\n", strerror(errno));
    }

    int close_fd;
    memcpy(&close_fd, (int *)(ans.return_val + sizeof(int)), sizeof(int));

    return close_fd;
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

    return_type ans;
    ans = make_remote_call(destAddr,
              destPort,
              "fsRead", 3,
              sizeof(int),
              (void *)&fd,
              count,
              buf,
              sizeof(unsigned int),
              (void *)&count);

    printf("Got response from fsRead RPC.\n");
    int sz = ans.return_size;

    int readErrno;
    memcpy(&readErrno, (int *)ans.return_val, sizeof(int));

    int bytes;
    memcpy(&bytes, (int *)(ans.return_val + sizeof(int)), sizeof(int));

    if(readErrno != 0) {
        errno = readErrno;
        printf("fsRead() Error: %s\n", strerror(readErrno));
        return bytes;
    }

    memcpy(buf, (ans.return_val + sizeof(int) + sizeof(int)), count);

    return bytes;
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

    return_type ans;
    ans = make_remote_call(destAddr,
              destPort,
              "fsWrite", 3,
              sizeof(int),
              (void *)&fd,
              count,
              buf,
              sizeof(unsigned int),
              (void *)&count);

    printf("Got response from fsWrite RPC.\n");
    int sz = ans.return_size;

    int writeErrno;
    memcpy(&writeErrno, (int *)ans.return_val, sizeof(int));

    if(writeErrno != 0) {
        errno = writeErrno;
        printf("fsWrite() Error: %s\n", strerror(errno));
    }

    int bytes;
    memcpy(&bytes, (int *)(ans.return_val + sizeof(int)), sizeof(int));

    return bytes;
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

    char *root_path = (char *) malloc(strlen(localDirName) + 1);
    memcpy(root_path, name, strlen(localDirName));

    if(strcmp(root_path, localDirName) != 0) {
        errno = ENOENT;
        return -1;
    }

    return_type ans;
    ans = make_remote_call(destAddr,
              destPort,
              "fsRemove", 1,
              strlen(name) + 1,
              (void *)name);

    printf("Got response from fsRemove RPC.\n");
    int sz = ans.return_size;

    int removeErrno;
    memcpy(&removeErrno, (int *)ans.return_val, sizeof(int));

    if(removeErrno != 0) {
        errno = removeErrno;
        printf("fsRemove() Error: %s\n", strerror(errno));
    }

    int ret;
    memcpy(&ret, (int *)(ans.return_val + sizeof(int)), sizeof(int));

    return ret;
}

