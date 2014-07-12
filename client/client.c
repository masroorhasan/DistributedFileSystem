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

    // Call fsMount from File System API
    fsMount(argv[1], atoi(argv[2]), "Warren Smith");
    fsUnmount("Warren Smith");

    return 0;
}
