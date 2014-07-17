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

		int ret_closedir = fsCloseDir(fd);
		printf("Closing directory returned: %i\n", ret_closedir);

    folder_path = "root/one";
    fd = fsOpenDir(folder_path);


		/* Read test case.
    if(fd != NULL) {
        struct fsDirent *fdent = NULL;

        fdent = fsReadDir(fd);
        printf("fsReadDir return: %s, %d\n", fdent->entName, (int)(fdent->entType));

        ret = fsCloseDir(fd);
        printf("fsCloseDir ret %i\n", ret);
    }
		*/
    return 0;
}
