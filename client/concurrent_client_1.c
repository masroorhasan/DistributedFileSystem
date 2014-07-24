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
    time_t ltime;
    ltime=time(NULL);
    printf("\n%s",asctime( localtime(&ltime) ) );
}

/*
 * Simple locking test case. Get a lock on a file and writes a string.
 * Releases after waiting 15 seconds and performing writes.
 * Contents of file should be W4W5 when run concurrently with concurrent_client_2.c.
 */
int main(int argc, char *argv[]) {

    fsMount(argv[1], atoi(argv[2]), "root");

    timestamp();
    printf("Attempting to open one.txt in grandfather directory\n");
    int fd = fsOpen("root/test.txt", 1);

    timestamp();
    printf("Opened one.txt with fd: %i\n", fd);
    timestamp();
    printf("Sleeping for 15 seconds.\n");
    sleep(15);

    timestamp();
    printf("Issuing writes to file\n");

    char *buffer = "W1";
    int w_return;
    w_return = fsWrite(fd, buffer, strlen(buffer) + 1);
    printf("Write one code: %i\n", w_return);

    buffer = "W2";
    w_return = fsWrite(fd, buffer, strlen(buffer) + 1);
    printf("Write two code: %i\n", w_return);

    buffer = "W3";
    w_return = fsWrite(fd, buffer, strlen(buffer) + 1);
    printf("Write three code: %i\n", w_return);

    timestamp();
    printf("Closing one.txt\n");
    int close_fd = fsClose(fd);
    timestamp();
    printf("Closed with code: %i\n", close_fd);
}
