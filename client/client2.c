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
    printf("fsMount(): %d\n", fsMount(argv[1], atoi(argv[2]), dirname));


    int fd = fsOpen("leaf/warren.txt", 1);
    char buf[256] = "Warren";
    fsWrite(fd, buf, 256);

    printf("Sleeping for 15 seconds\n");
    sleep(15);

    printf("Closing fd.\n");
    int close = fsClose(fd);
    int status = fsRemove("leaf/warren.txt");
    printf("Here is the status: %i", status);

}

