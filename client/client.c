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

    int fd = fsOpen("leaf/one/test1.txt", 1);
    printf("Opened leaf/one/test1.txt FD: %i\n", fd);

    int fd2 = fsOpen("leaf/one/test1.txt", 1);
    printf("Opened leaf/one/test1.txt FD: %i\n", fd2);

    int w_return;


    char *buffer = "W1";
    w_return = fsWrite(fd, buffer, strlen(buffer) + 1);

    buffer = "W2";
    w_return = fsWrite(fd, buffer, strlen(buffer) + 1);

    buffer = "W3";
    w_return = fsWrite(fd, buffer, strlen(buffer) + 1);

    buffer = "W4";
    w_return = fsWrite(fd2, buffer, strlen(buffer) + 1);

    // buffer = "W5";
    // w_return = fsWrite(fd2, buffer, strlen(buffer) + 1);

    // buffer = "W6";
    // w_return = fsWrite(fd2, buffer, strlen(buffer) + 1);


    int c_return = fsClose(fd);

    fsUnmount("leaf");
    return 0;
}
