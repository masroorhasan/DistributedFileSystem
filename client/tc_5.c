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

    printf("fsMount(): %d\n", fsMount(argv[1], atoi(argv[2]), "root"));

    printf("Unmounting root.\n");
    if(fsUnmount("root") < 0) {
	      perror("fsUnmount"); exit(1);
    }

    printf("fsMount(): %d\n", fsMount(argv[1], atoi(argv[2]), "leaf"));

    printf("Unmounting leaf.\n");
    if(fsUnmount("leaf") < 0) {
	      perror("fsUnmount"); exit(1);
    }

    int open = fsOpen("root/one.txt", 1);
    printf("Attempted to open with close path. Code: %i\n", open);

    printf("fsMount(): %d\n", fsMount(argv[1], atoi(argv[2]), "warren"));

    printf("Unmounting warren.\n");
    if(fsUnmount("warren") < 0) {
	      perror("fsUnmount"); exit(1);
    }
    printf("Attempting to unmount warren twice.\n");
    if(fsUnmount("warren") < 0) {
	      perror("Error unmounting"); exit(1);
    }
    return 0;
}
