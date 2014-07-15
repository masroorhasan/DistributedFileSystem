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
    // fsUnmount("root");

    char* folder_path = "root";
    FSDIR* fd = fsOpenDir(folder_path);
    
    struct fsDirent *fdent = NULL;
  //   for(fdent = fsReadDir(fd); fdent != NULL; fdent = fsReadDir(fd)) {
		// printf("\t %s, %d\n", fdent->entName, (int)(fdent->entType));
  //   }
    
    fdent = fsReadDir(fd);
    // printf("\t %s, %d\n", fdent->entName, (int)(fdent->entType));
    
    // int ret = fsCloseDir(fd);
    // printf("fsCloseDir ret: %i\n", ret);

    // FSDIR* fd2 = fsOpenDir(folder_path);
	// fdent = fsReadDir(fd);
    // printf("\t %s, %d\n", fdent->entName, (int)(fdent->entType));    
    
    // int ret = fsCloseDir(fd);
    // printf("fsCloseDir ret: %i\n", ret);


    return 0;
}
