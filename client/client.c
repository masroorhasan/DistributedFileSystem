/* Mahesh V. Tripunitara
   University of Waterloo
   Part of a simplified RPC implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#include "ece454_fs.h"

void printBuf(char *buf, int size) {
    /* Should match the output from od -x */
    int i;
    for(i = 0; i < size; ) {
    if(i%16 == 0) {
        printf("%08o ", i);
    }

    int j;
    for(j = 0; j < 16;) {
        int k;
        for(k = 0; k < 2; k++) {
        if(i+j+(1-k) < size) {
            printf("%02x", (unsigned char)(buf[i+j+(1-k)]));
        }
        }

        printf(" ");
        j += k;
    }

    printf("\n");
    i += j;
    }
}

int main(int argc, char *argv[]) {
    if(argc < 4) {
        fprintf(stderr, "usage: %s <srv-ip/name> <srv-port> <local dir name>\n", argv[0]);
        exit(1);
    }
    
    char *dirname = argv[3];
    int mounted = fsMount(argv[1], atoi(argv[2]), dirname);
    printf("fsMount(): %d\n", mounted);

    // dirname = "hasan";
    // mounted = fsMount(argv[1], atoi(argv[2]), dirname);
    // printf("fsMount(): %d\n", mounted);    

    // int unmounted = fsUnmount(dirname);
    // printf("fsUnmount(): %d\n", unmounted);    

    // dirname = "masroor";
    // unmounted = fsUnmount(dirname);    
    // printf("fsUnmount(): %d\n", unmounted);

    char *path = "masroor/one";
    FSDIR *fsdir = fsOpenDir(path);

    struct fsDirent *fdent = NULL;
    for(fdent = fsReadDir(fsdir); fdent != NULL; fdent = fsReadDir(fsdir)) {
        printf("\t %s, %d\n", fdent->entName, (int)(fdent->entType));
    }

    int closdir = fsCloseDir(fsdir);
    printf("closedir %i\n", closdir);

    int fd = fsOpen("masroor/test1.txt", 1);
    printf("fd for fsOpen %i\n", fd);

    printf("Issuing writes to file\n");

    char *buffer = "Masroor";
    int w_return = fsWrite(fd, buffer, strlen(buffer) + 1);
    printf("Write one code: %i\n", w_return);

    char *dirname2 = "hasan";
    char *server = "192.168.2.15";
    int port = 10001;
    // mounted = fsMount(argv[1], atoi(argv[2]), dirname2);
    mounted = fsMount(server, port, dirname2);
    printf("fsMount(): %d\n", mounted);

    int close = fsClose(fd);
    printf("closing fd : %i\n", close);

    int unmount = fsUnmount(dirname);
    printf("unmount %i\n", unmount);

    // printf("sleeping for 5s\n");
    // sleep(2);
    path = "hasan";
    fsdir = fsOpenDir(path);

    fdent = NULL;
    for(fdent = fsReadDir(fsdir); fdent != NULL; fdent = fsReadDir(fsdir)) {
        printf("\t %s, %d\n", fdent->entName, (int)(fdent->entType));
    }

    closdir = fsCloseDir(fsdir);
    printf("closedir %i\n", closdir);


    int fd2 = fsOpen("hasan/test2.txt", 1);
    printf("fd2 for fsOpen %i\n", fd2);

    printf("Issuing writes to file\n");

    buffer = "Hasan";
    w_return = fsWrite(fd2, buffer, strlen(buffer) + 1);
    printf("Write one code: %i\n", w_return);

    close = fsClose(fd2);
    printf("closing fd : %i\n", close);

    unmount = fsUnmount(dirname2);
    printf("unmount %i\n", unmount);
    
    // int fd = fsOpen("root/test.txt", 1);
    // printf("first fd %i\n", fd);

    // printf("Issuing writes to file\n");

    // char *buffer = "W7";
    // int w_return;
    // w_return = fsWrite(fd, buffer, strlen(buffer) + 1);
    // printf("Write one code: %i\n", w_return);

    // fsClose(fd);

    // fd = fsOpen("leaf/one/one.txt", 1);
    // printf("second fd %i\n", fd);

    // fd = fsOpen("leaf/one/one.txt", 1);
    // printf("third fd %i\n", fd);


    // printf("Attempting to open test1.txt in grandfather directory\n");
    // int fd = fsOpen("root/test1.txt", 1);


    // printf("Opened test.txt with fd: %i\n", fd);


    // printf("Issuing writes to file\n");
    // int w_return;
    // char *buffer = "Client4W1";
    // w_return = fsWrite(fd, buffer, strlen(buffer) + 1);
    // printf("Write one code: %i\n", w_return);

    // buffer = "Client4W2";
    // w_return = fsWrite(fd, buffer, strlen(buffer) + 1);
    // printf("Write two code: %i\n", w_return);


    // printf("Closing test1.txt\n");
    // int close_fd = fsClose(fd);

    // printf("Closed with code: %i\n", close_fd);

    /*
    
    FSDIR *fd = fsOpenDir(dirname);
    if(fd == NULL) {
        perror("fsOpenDir"); exit(1);
    }

    struct fsDirent *fdent = NULL;
    for(fdent = fsReadDir(fd); fdent != NULL; fdent = fsReadDir(fd)) {
        printf("\t %s, %d\n", fdent->entName, (int)(fdent->entType));
    }

    if(errno != 0) {
        perror("fsReadDir");
    }

    printf("fsCloseDir(): %d\n", fsCloseDir(fd));

    int ff = open("/dev/urandom", 0);
    if(ff < 0) {
        perror("open(/dev/urandom)"); exit(1);
    }
    else printf("open(): %d\n", ff);

    char fname[256];
    sprintf(fname, "%s/", dirname);
    if(read(ff, (void *)(fname+strlen(dirname)+1), 10) < 0) {
        perror("read(/dev/urandom)"); exit(1);
    }

    int i;
    for(i = 0; i < 10; i++) {
        //printf("%d\n", ((unsigned char)(fname[i]))%26);
        fname[i+strlen(dirname)+1] = ((unsigned char)(fname[i+strlen(dirname)+1]))%26 + 'a';
    }
    fname[10+strlen(dirname)+1] = (char)0;
    printf("Filename to write: %s\n", (char *)fname);

    char buf[256];
    if(read(ff, (void *)buf, 256) < 0) {
        perror("read(2)"); exit(1);
    }

    printBuf(buf, 256);

    printf("close(): %d\n", close(ff));

    ff = fsOpen(fname, 1);
    if(ff < 0) {
        perror("fsOpen(write)"); exit(1);
    }

    if(fsWrite(ff, buf, 256) < 256) {
        fprintf(stderr, "fsWrite() wrote fewer than 256\n");
    }

    if(fsClose(ff) < 0) {
        perror("fsClose"); exit(1);
    }

    char readbuf[256];
    if((ff = fsOpen(fname, 0)) < 0) {
        perror("fsOpen(read)"); exit(1);
    }

    int readcount = -1;

    if((readcount = fsRead(ff, readbuf, 256)) < 256) {
        fprintf(stderr, "fsRead() read fewer than 256\n");
    }

    if(memcmp(readbuf, buf, readcount)) {
        fprintf(stderr, "return buf from fsRead() differs from data written!\n");
    } else {
        printf("fsread(): return buf identical to data written upto %d bytes.\n", readcount);
    }

    if(fsClose(ff) < 0) {
        perror("fsClose"); exit(1);
    }

    // printf("fsRemove(%s): %d\n", fname, fsRemove(fname));

    if(fsUnmount(dirname) < 0) {
        perror("fsUnmount"); exit(1);
    }
    */
    return 0;
}

