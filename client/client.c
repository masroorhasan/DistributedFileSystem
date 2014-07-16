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


int main(int argc, char *argv[]) {
    if(argc < 3) {
	      printf("usage: %s <server ip/name> <server port>\n", argv[0]);
	      return 0;
    }

    fsMount(argv[1], atoi(argv[2]), "root");

    char* folder_path = "root";
    FSDIR* fd = fsOpenDir(folder_path);
    printf("Here is fd received by client on fsOpenDir: %x\n", fd);

    struct fsDirent *fdent = NULL;

    // Calling and printing fsReadDir call
    fdent = fsReadDir(fd);
    fdent = fsReadDir(fd);

    // Calling and printing fsCloseDir call
    int ret = fsCloseDir(fd);

    // fsUnmount("root");

    return 0;
}
