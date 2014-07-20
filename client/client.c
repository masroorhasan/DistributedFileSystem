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

    int fdr = fsOpen("leaf/open.txt", 1);
    printf("Opened leaf/open.txt FD: %i\n", fdr);

    char buff[16];
    int r_return;
    r_return = fsRead(fdr, (void *)buff, 10);
    printf("bytes read %i into buffer %s\n", r_return, buff);

    // int fdw = fsOpen("leaf/one/test1.txt", 1);
    // printf("Opened leaf/one/test1.txt FD: %i\n", fdw);

    
    // char *buffer = "W1";
    // int w_return;
    // w_return = fsWrite(fdw, buffer, strlen(buffer) + 1);

    // Perform fsread again

    // int c_return = fsClose(fdw);

    // char *file_to_remove = "leaf/test.txt";
    // int remove_ret = fsRemove(file_to_remove);
    // printf("remove ret value %i\n", remove_ret);

    // char *file_to_remove = "leaf/one/delete/delete.txt";
    // int remove_ret = fsRemove(file_to_remove);
    // printf("remove ret value %i\n", remove_ret);

    // char *dir_to_remove = "leaf/one/delete";
    // remove_ret = fsRemove(dir_to_remove);
    // printf("remove ret value %i\n", remove_ret);    

    fsUnmount("leaf");
    return 0;
}
