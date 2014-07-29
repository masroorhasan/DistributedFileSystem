#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    printf("fsMount(): %d\n", fsMount("129.97.56.12", 10050, "root"));
    printf("fsMount(): %d\n", fsMount("129.97.56.13", 10009, "leaf"));

    FSDIR *fd = fsOpenDir("root");
    struct fsDirent *fdent = NULL;

    printf("Reading from root.\n");
    for(fdent = fsReadDir(fd); fdent != NULL; fdent = fsReadDir(fd)) {
      	printf("%s, %d\n", fdent->entName, (int)(fdent->entType));
    }

    FSDIR *fd2 = fsOpenDir("leaf");
    fdent = NULL;

    printf("Reading from leaf.\n");
    for(fdent = fsReadDir(fd); fdent != NULL; fdent = fsReadDir(fd)) {
      	printf("%s, %d\n", fdent->entName, (int)(fdent->entType));
    }

    printf("Opening file on root.\n");
    int ff = fsOpen("root/rootWrite.txt", 1);

    printf("Opening file on leaf.\n");
    int ff2 = fsOpen("leaf/leafWrite.txt", 1);

    char* buf = "Warren writing to root";
    fsWrite(ff, buf, 256);

    char* buf2 = "Warren writing to leaf";
    fsWrite(ff2, buf2, 256);

    char* buf3 = (char *) malloc(256);
    char* buf4 = (char *) malloc(256);

    printf("Reading from root.\n");
    int readcount = 0;

    if((readcount = fsRead(ff, buf3, 256)) < 256) {
        printf("Read from root failed.\n");
    }

    printf("Reading from leaf.\n");
    if((readcount = fsRead(ff2, buf4, 256)) < 256) {
        printf("Read from leaf failed.\n");
    }

    printf("Attempting to unmount root.\n");
    if(fsUnmount("root") < 0) {
	      perror("fsUnmount root"); exit(1);
    }

    printf("Attempting to unmount leaf.\n");
    if(fsUnmount("leaf") < 0) {
	      perror("fsUnmount leaf"); exit(1);
    }

    printf("fsCloseDir root: %d\n", fsCloseDir(fd));
    printf("fsCloseDir leaf: %d\n", fsCloseDir(fd2));

    return 0;
}
