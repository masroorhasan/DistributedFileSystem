#include "ece454_fs_client.h"

bool checkMountedState(const char *localdirname) {
    printf("localdirname: %s\n", localdirname);

    int i = 0;
    bool found_slash = false;
    char *fwdslash = "/";
    for(; i < strlen(localdirname); i++) {
        if(localdirname[i] == '/') {
            found_slash = true;
            break;
        }
    }

    char *parsed_folder = NULL;
    if(found_slash == true) {
        parsed_folder = (char *) malloc(sizeof(char) * (i+1));
        memset(parsed_folder, 0, sizeof(char) * (i+1));
        memcpy(parsed_folder, localdirname, sizeof(char)*i);
    } else {
        parsed_folder = (char *) malloc(strlen(localdirname) + 1);
        memset(parsed_folder, 0, sizeof(char) * (i+1));
        memcpy(parsed_folder, localdirname, strlen(localdirname));
    }

    i = 0;
    for(; i < mounted_index; i++) {
        char *root_path = (char *) malloc(strlen(mounted_list[i].localDirName) + 1);
        memset(root_path, 0, strlen(mounted_list[i].localDirName) + 1);
        memcpy(root_path, parsed_folder, strlen(mounted_list[i].localDirName));

        if(strcmp(mounted_list[i].localDirName, root_path) == 0) {
            return mounted_list[i].mounted;
        }
    }

    return false;
}

/*
 * Get server ip and port # given a local folder name from list
 */
int getMountedServerInfo(const char *localdirname) {
    if(mounted_index == 0) return -1;
    
    int i = 0;
    bool found_slash = false;
    char *fwdslash = "/";
    for(; i < strlen(localdirname); i++) {
        if(localdirname[i] == '/') {
            found_slash = true;
            break;
        }
    }

    char *parsed_folder = NULL;
    if(found_slash == true) {
        parsed_folder = (char *) malloc(sizeof(char) * (i+1));
        memset(parsed_folder, 0, sizeof(char) * (i+1));
        memcpy(parsed_folder, localdirname, sizeof(char)*i);
    } else {
        parsed_folder = (char *) malloc(strlen(localdirname) + 1);
        memset(parsed_folder, 0, sizeof(char) * (i+1));
        memcpy(parsed_folder, localdirname, strlen(localdirname));
    }

    i = 0;
    for(; i < mounted_index; i++) {
        char *root_path = (char *) malloc(strlen(mounted_list[i].localDirName) + 1);
        memset(root_path, 0, strlen(mounted_list[i].localDirName) + 1);
        memcpy(root_path, parsed_folder, strlen(mounted_list[i].localDirName));

        if(strcmp(mounted_list[i].localDirName, root_path) == 0) {
            return i;
        }
    }

    return -1;
}

/*
 * Add new mapping of server ip and port # to local folder name to list
 */
int addToMountList(const char *serverip, const unsigned int port, const char *localdirname) {
    int idx = mounted_index;

    mounted_list_type node;

    if(serverip == NULL || port == 0 || localdirname == NULL) return -1;
    
    node.server_ip =  (char *) malloc(sizeof(char) * (strlen(serverip) +1));
    memcpy(node.server_ip, serverip, (strlen(serverip) +1));

    node.port = port;

    node.localDirName = (char *) malloc(sizeof(char) * (strlen(localdirname) +1));
    memcpy(node.localDirName, localdirname, (strlen(localdirname) +1));

    node.mounted = false;

    mounted_list[mounted_index] = node;
    mounted_index++;

    return idx;
}

/*
 * Set mounted state of local folder name mapping to server address 
 */
int setMountedState(const char *localdirname) {
    if(mounted_index == 0 || localdirname == NULL) return -1;

    int i = 0;
    for(; i < mounted_index; i++) {
        if(strcmp(mounted_list[i].localDirName, localdirname) == 0) {
            if(mounted_list[i].mounted == false) {
                mounted_list[i].mounted = true;
                return i;
            }
        } 
    }

    return -1;
}

/*
 * Set unmounted state of local folder name mapping to server address 
 */
int setUnmountedState(const char *localdirname) {
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

/*
 * print mapping between local folder name, port and server address
 */
void printMountedList() {
    int i = 0;
    for(; i < mounted_index; i++) {
         printf("serverip %s, port %i, localdir %s, mounted %i\n", 
            mounted_list[i].server_ip, mounted_list[i].port, mounted_list[i].localDirName,
            mounted_list[i].mounted);
    }
}

/*
 * Add new mapping of FSDIR* to local folder name in list
 */
int addToFsdirList(FSDIR *dir, const char* localdirname) {
    int idx = opened_fsdir_index;
    fsdir_list_type node;

    int i = 0;
    bool found_slash = false;
    char *fwdslash = "/";
    for(; i < strlen(localdirname); i++) {
        if(localdirname[i] == '/') {
            found_slash = true;
            break;
        }
    }

    char *parsed_folder = NULL;
    if(found_slash == true) {
        parsed_folder = (char *) malloc(sizeof(char) * (i+1));
        memset(parsed_folder, 0, sizeof(char) * (i+1));
        memcpy(parsed_folder, localdirname, sizeof(char)*i);
    } else {
        parsed_folder = (char *) malloc(strlen(localdirname) + 1);
        memset(parsed_folder, 0, sizeof(char) * (i+1));
        memcpy(parsed_folder, localdirname, strlen(localdirname));
    }
    
    node.localdirname = (char *)malloc(strlen(parsed_folder) + 1);
    memset(node.localdirname, 0, strlen(parsed_folder) + 1);
    memcpy(node.localdirname, parsed_folder, strlen(parsed_folder) + 1);

    node.remotefsdir = (FSDIR *)malloc(sizeof(FSDIR));
    memcpy(node.remotefsdir, dir, sizeof(FSDIR));

    fsdir_list[opened_fsdir_index] = node;
    opened_fsdir_index++;

    return idx;
}

/*
 * Return index of local folder name from list given an FSDIR pointer
 */
int getFoldernameFromFSDIR(FSDIR *dir) {
    int i = 0;
    int fsdir = *dir;
    for(; i < opened_fsdir_index; i++) {
        int list_fsdir = *(fsdir_list[i].remotefsdir);
        if(fsdir == list_fsdir) {
            return i;
        }
    }

    return -1;
}

/*
 * Utility function to print mapping of remote FSDIR and local folder name
 */
void printFSDIRList() {
    int i = 0;
    for(; i < opened_fsdir_index; i++) {
         printf("localdirName %s, remotefsdir %i\n", 
            fsdir_list[i].localdirname, *(fsdir_list[i].remotefsdir));
    }
}

/*
 * Adds a mapping between remote file descriptor to local folder name to list
 */
int addTofdList(int fd, const char *localdirname) {
    int idx = opened_fd_index;
    fd_list_type node;

    int i = 0;
    bool found_slash = false;
    char *fwdslash = "/";
    for(; i < strlen(localdirname); i++) {
        if(localdirname[i] == '/') {
            found_slash = true;
            break;
        }
    }

    char *parsed_folder = NULL;
    if(found_slash == true) {
        parsed_folder = (char *) malloc(sizeof(char) * (i+1));
        memset(parsed_folder, 0, sizeof(char) * (i+1));
        memcpy(parsed_folder, localdirname, sizeof(char)*i);
    } else {
        parsed_folder = (char *) malloc(strlen(localdirname) + 1);
        memset(parsed_folder, 0, sizeof(char) * (i+1));
        memcpy(parsed_folder, localdirname, strlen(localdirname));
    }

    node.localdirname = (char *)malloc(strlen(parsed_folder) + 1);
    memset(node.localdirname, 0, strlen(parsed_folder) + 1);
    memcpy(node.localdirname, parsed_folder, strlen(parsed_folder) + 1);

    node.remotefd = fd;

    fd_list[opened_fd_index] = node;
    opened_fd_index++;

    return idx;
}

/*
 * Get index of local folder name given local file descriptor from list
 */
int getFoldernameFromFD(int localfd) {
    if(localfd == -1) return -1;

    int i = 0;
    for(; i < opened_fd_index; i++) {
        if(localfd == fd_list[i].remotefd) {
            return i;
        }
    }

    return -1;
}
/*
 * Utility function to print List containing remote file descriptors
 */
void printFDList() {
    int i = 0;
    for(; i < opened_fd_index; i++) {
         printf("localdirName %s, remotefd %i\n", 
            fd_list[i].localdirname, (fd_list[i].remotefd));
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
    if(srvIpOrDomName == NULL || srvPort == 0 || localFolderName == NULL) return -1;
    
    // Check that we aren't mounted
    if(checkMountedState(localFolderName)) return -1;

    int mount_idx = addToMountList(srvIpOrDomName, srvPort, localFolderName);
    if(mount_idx == -1) {
        //set errno
        return -1;
    }

    return_type ans;
    ans = make_remote_call(
               mounted_list[mount_idx].server_ip,
		       mounted_list[mount_idx].port,
		       "fsMount", 0);

    printf("Got response from fsMount RPC.\n");

    int size = ans.return_size;
    int value;
    memcpy(&value, (int *)ans.return_val, sizeof(int));

    if (value == 0) {
        int mounted_idx = setMountedState(localFolderName);
        if(mounted_idx != -1) 
            printf("Folder %s was successfully mounted.\n", localFolderName);
    } else {
        int mountErrno;
        memcpy(&mountErrno, (int *)(ans.return_val + sizeof(int)), sizeof(int));

        errno = mountErrno;
        printf("fsMount() Error: %s \n", strerror(errno));
    }

    printMountedList();

    return value;
}

/*
 * Unmounts a remote server folder.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsUnmount(const char *localFolderName) {
    printf("fsUnmount\n");
    if(localFolderName == NULL) return -1;

    // Check that we're mounted
    if(!checkMountedState(localFolderName)) return -1;

    int mounted_idx = getMountedServerInfo(localFolderName);

    return_type ans;
    ans = make_remote_call(
                mounted_list[mounted_idx].server_ip,
                mounted_list[mounted_idx].port,
               "fsUnmount", 0);

    printf("Got response from fsUnmount RPC.\n");

    int size = ans.return_size;
    int value = *(int *)(ans.return_val);

    if (value == 0) {
        // mounted = false;
        int unmounted_idx = setUnmountedState(localFolderName);
        
        if(unmounted_idx != -1) 
            printf("Folder %s was successfully unmounted.\n", localFolderName);
    }

    printMountedList();

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
    printf("fsOpenDir()\n");
    if(folderName == NULL) return NULL;
    
    // Check that we're mounted
    if(!checkMountedState(folderName)) return NULL;
    int mounted_idx = getMountedServerInfo(folderName);

    return_type ans;
    ans = make_remote_call(
                mounted_list[mounted_idx].server_ip,
                mounted_list[mounted_idx].port,
                "fsOpenDir", 1,
                strlen(folderName) + 1,
                folderName);

    printf("Got response from fsOpenDir RPC.\n");

    int size = ans.return_size;

    int openDirErrno;
    memcpy(&openDirErrno, (int *)ans.return_val, sizeof(int));

    FSDIR *dir = (FSDIR *) malloc(sizeof(FSDIR));
    FSDIR *localdir_ptr = (FSDIR *) malloc(sizeof(FSDIR));
    if(openDirErrno == 0) {
        memcpy(dir, (FSDIR *)(ans.return_val + sizeof(int)), sizeof(FSDIR));
        int localfsdir = addToFsdirList(dir, folderName);
        
        memcpy(localdir_ptr, &localfsdir, sizeof(FSDIR));

    } else {
        dir = NULL;
        errno = openDirErrno;
        printf("fsDirOpen() Error: %s \n", strerror(errno));
    }

    return localdir_ptr;
}

/*
 * Counterpart to fsOpenDir().
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsCloseDir(FSDIR * folder) {
    printf("fsCloseDir()\n");
    
    int fsdir_idx = *folder;
    printf("fsdir_idx %i\n", fsdir_idx);
    if(fsdir_idx == -1) return -1;

    // Check that we're mounted
    if(!checkMountedState(fsdir_list[fsdir_idx].localdirname)) return -1;
    int mounted_idx = getMountedServerInfo(fsdir_list[fsdir_idx].localdirname);  

    return_type ans;
    ans = make_remote_call(
                mounted_list[mounted_idx].server_ip,
                mounted_list[mounted_idx].port,
                "fsCloseDir", 1,
                sizeof(FSDIR),
                fsdir_list[fsdir_idx].remotefsdir);

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
    printf("fsReadDir()\n");
    
    int fsdir_idx = *folder;
    printf("fsdir_idx %i\n", fsdir_idx);
    if(fsdir_idx == -1) return NULL;

    // Check that we're mounted
    if(!checkMountedState(fsdir_list[fsdir_idx].localdirname)) return NULL;
    int mounted_idx = getMountedServerInfo(fsdir_list[fsdir_idx].localdirname); 

    return_type ans;
    ans = make_remote_call(
            mounted_list[mounted_idx].server_ip,
            mounted_list[mounted_idx].port,
            "fsReadDir", 1,
            sizeof(FSDIR),
            fsdir_list[fsdir_idx].remotefsdir);

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
    printf("fsOpen\n");
    if(fname == NULL) return -1; 

    // Check that we're mounted
    if(!checkMountedState(fname)) return -1;
    int mounted_idx = getMountedServerInfo(fname);

    int fname_sz = strlen(fname) + 1;
    int mode_sz = sizeof(int);

    int uid = -1;

    return_type ans;
    ans = make_remote_call(
            mounted_list[mounted_idx].server_ip,
            mounted_list[mounted_idx].port,
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
        ans = make_remote_call(
                mounted_list[mounted_idx].server_ip,
                mounted_list[mounted_idx].port,
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

    //put fd in fdlist
    int localfd = addTofdList(open_fd, fname);
    return localfd;
}


/*
 * The counterpart to open(). The argument file descriptor is no longer
 * valid after this call.
 *
 * Returns 0 on success.
 * Returns -1 on failure and sets errno.
 */
extern int fsClose(int fd) {
    printf("fsClose()\n");

    int fd_idx = fd; 
    if(fd_idx == -1) return -1;

    // Check that we're mounted
    if(!checkMountedState(fd_list[fd_idx].localdirname)) return -1;
    int mounted_idx = getMountedServerInfo(fd_list[fd_idx].localdirname); 

    return_type ans;
    ans = make_remote_call(
                mounted_list[mounted_idx].server_ip,
                mounted_list[mounted_idx].port,
                "fsClose", 1,
                sizeof(int),
                &(fd_list[fd_idx].remotefd));

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
    printf("fsRead()\n");

    int fd_idx = fd; 
    if(fd_idx == -1) return -1;

    // Check that we're mounted
    if(!checkMountedState(fd_list[fd_idx].localdirname)) return -1;
    int mounted_idx = getMountedServerInfo(fd_list[fd_idx].localdirname); 

    return_type ans;
    ans = make_remote_call(
                mounted_list[mounted_idx].server_ip,
                mounted_list[mounted_idx].port,
                "fsRead", 3,
                sizeof(int),
                &(fd_list[fd_idx].remotefd),
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
    printf("fsWrite()\n");
    int fd_idx = fd; 
    if(fd_idx == -1) return -1;

    // Check that we're mounted
    if(!checkMountedState(fd_list[fd_idx].localdirname)) return -1;
    int mounted_idx = getMountedServerInfo(fd_list[fd_idx].localdirname); 

    return_type ans;
    ans = make_remote_call(
                mounted_list[mounted_idx].server_ip,
                mounted_list[mounted_idx].port,
                "fsWrite", 3,
                sizeof(int),
                &(fd_list[fd_idx].remotefd),
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
    printf("fsRemove()\n");

    if(name == NULL) return -1;

    // Check that we're mounted
    if(!checkMountedState(name)) return -1;
    int mounted_idx = getMountedServerInfo(name);

    printf("Issuing fsRemove Request\n");

    // Client send -1 on first request
    int uid = -1;
    return_type ans;
    ans = make_remote_call(
                mounted_list[mounted_idx].server_ip,
                mounted_list[mounted_idx].port,
                "fsRemove", 2,
                strlen(name) + 1,
                (void *)name,
                sizeof(int),
                &uid);

    printf("Got response from fsRemove RPC.\n");
    int sz = ans.return_size;

    // If NACK sent back from server, file was locked, keep trying    
    waiting_state state;
    memcpy(&state, (int *)ans.return_val, sizeof(int));
    printf("state: %i\n", state);

    memcpy(&uid, (int *)(ans.return_val + sizeof(int)), sizeof(int));
    printf("uid: %i\n", uid);

    while(state == NACK && uid != -1) {
        ans = make_remote_call(
                mounted_list[mounted_idx].server_ip,
                mounted_list[mounted_idx].port,
                "fsRemove", 2,
                strlen(name) + 1,
                (void *)name,
                sizeof(int),
                &uid);

        memcpy(&state, (int *)ans.return_val, sizeof(int));
        printf("state: %i\n", state);

        memcpy(&uid, (int *)(ans.return_val + sizeof(int)), sizeof(int));
        printf("uid: %i\n", uid);

        printf("sleeping for 2s...\n");
        sleep(2);
    } 


    int removeErrno;
    memcpy(&removeErrno, (int *)(ans.return_val + (sizeof(int)*2)), sizeof(int));

    if(removeErrno != 0) {
        errno = removeErrno;
        printf("fsRemove() Error: %s\n", strerror(errno));
    }

    int ret;
    memcpy(&ret, (int *)(ans.return_val + (sizeof(int)*3)), sizeof(int));

    return ret;
}

