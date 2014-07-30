#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#include "ece454_fs.h"

void timestamp() {
//    time_t ltime;
//    ltime=time(NULL);
//    printf("\n%s",asctime( localtime(&ltime) ) );
}

int main(int argc, char *argv[]) {

    if(argc < 3) {
        fprintf(stderr, "usage: %s <srv-ip/name> <srv-port>\n", argv[0]);
        exit(1);
    }

    printf("fsMount(): %d\n", fsMount(argv[1], atoi(argv[2]), "root"));

    timestamp();
    printf("Attempting to open one.txt in grandfather directory\n");
    int fd = fsOpen("root/test.txt", 1);

    timestamp();
    printf("Opened one.txt with fd: %i\n", fd);

    timestamp();
    printf("Issuing writes to file\n");
    int w_return;
    char *buffer = "W6";
    w_return = fsWrite(fd, buffer, strlen(buffer) + 1);
    printf("Write one code: %i\n", w_return);

    timestamp();
    printf("Closing one.txt\n");
    int close_fd = fsClose(fd);

    timestamp();
    printf("Closed with code: %i\n", close_fd);
}
