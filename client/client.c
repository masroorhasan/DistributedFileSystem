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
	      printf("Usage: %s <Server IP> <Server Port>\n", argv[0]);
	      return 0;
    }

    fsMount(argv[1], atoi(argv[2]), "leaf");

    char* folder_path = "leaf/one";
    FSDIR* fdir = fsOpenDir(folder_path);

    struct fsDirent *fdent = NULL;

    for(fdent = fsReadDir(fdir); fdent != NULL; fdent = fsReadDir(fdir)) {
        printf("%s, %d\n", fdent->entName, (int)(fdent->entType));
    }

    // Open bunch of files
    int fd = fsOpen("leaf/test.txt", 1);
    printf("open fd: %i\n", fd);

    int fd2 = fsOpen("leaf/one/test1.txt", 1);
    printf("open fd: %i\n", fd2);

    int fd3 = fsOpen("leaf/three/two.txt", 0);
    printf("open fd: %i\n", fd3);

    int fd4 = fsOpen("leaf/two/two.txt", 0);
    printf("open fd: %i\n", fd4);

    // Close those files
    int cls_fd = fsClose(fd);
    printf("closing fd %i with ret %i\n", fd, cls_fd);

    // int cls_fd2 = fsClose(fd2);
    // printf("closing fd %i with ret %i\n", fd2, cls_fd2);

    int cls_fd3 = fsClose(fd3);
    printf("closing fd %i with ret %i\n", fd3, cls_fd3);

    int cls_fd4 = fsClose(fd4);
    printf("closing fd %i with ret %i\n", fd4, cls_fd4);

    char *buf = "writing test";
    int write = fsWrite(fd2, buf, 256);

    // Closing directory
    int ret = fsCloseDir(fdir);
    printf("closedir returned: %i\n", ret);
    fsUnmount("leaf");
    return 0;
}
