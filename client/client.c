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
    // fsMount(argv[1], atoi(argv[2]), "leaf");


    char* folder_path = "leaf/one";
    FSDIR* fdir = fsOpenDir(folder_path);

    struct fsDirent *fdent = NULL;

    // for(fdent = fsReadDir(fdir); fdent != NULL; fdent = fsReadDir(fdir)) {
    //     printf("%s, %d\n", fdent->entName, (int)(fdent->entType));
    // }
    
    fdent = fsReadDir(fdir);
    printf("%s, %d\n", fdent->entName, (int)(fdent->entType));

    fdent = fsReadDir(fdir);
    printf("%s, %d\n", fdent->entName, (int)(fdent->entType));

    fdent = fsReadDir(fdir);
    printf("%s, %d\n", fdent->entName, (int)(fdent->entType));

    int fd = fsOpen("leaf/test.txt", 1);
    printf("open fd: %i\n", fd);

    int fd2 = fsOpen("leaf/one/test1.txt", 1);
    printf("open fd: %i\n", fd2);

    int fd3 = fsOpen("leaf/three/two.txt", 0);
    printf("open fd: %i\n", fd3);

    int fd4 = fsOpen("leaf/two/two.txt", 0);
    printf("open fd: %i\n", fd4);

    int ret = fsCloseDir(fdir);
    printf("closedir returned: %i\n", ret);
    fsUnmount("leaf");
    
      
    return 0;
}
