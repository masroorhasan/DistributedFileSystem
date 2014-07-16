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

    fsMount(argv[1], atoi(argv[2]), "root");

    char* folder_path = "root";
    FSDIR* fd = fsOpenDir(folder_path);

    if(fd != NULL) {
        struct fsDirent *fdent = NULL;

        fdent = fsReadDir(fd);
        printf("fsReadDir return: %s, %d\n", fdent->entName, (int)(fdent->entType));

        fdent = fsReadDir(fd);
        printf("fsReadDir return: %s, %d\n", fdent->entName, (int)(fdent->entType));

        fdent = fsReadDir(fd);
        printf("fsReadDir return: %s, %d\n", fdent->entName, (int)(fdent->entType));

        /*
         * Commented for the time being until NULL return implemented.
         */
        // struct fsDirent *fdent = NULL;
        // for(fdent = fsReadDir(fd); fdent != NULL; fdent = fsReadDir(fd)) {
        //    printf("\t %s, %d\n", fdent->entName, (int)(fdent->entType));
        // }
        
        int ret = fsCloseDir(fd);
        printf("fsCloseDir ret %i\n", ret);

        ret = fsCloseDir(fd);
        printf("fsCloseDir ret %i\n", ret);
    }

    // char *folder2 = "root2";
    // FSDIR *fd2 = fsOpenDir(folder2);
    // if(fd2 != NULL) {
    //     int ret2 = fsCloseDir(fd2);
    //     printf("fsCloseDir ret %i\n", ret2);        
    // }

    // fsUnmount("root");

    return 0;
}
