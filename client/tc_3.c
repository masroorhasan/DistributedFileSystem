#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ece454_fs.h"

int main(int argc, char *argv[]) {

    if(argc < 3) {
	      fprintf(stderr, "usage: %s <srv-ip/name> <srv-port>\n", argv[0]);
	      exit(1);
    }

    printf("fsMount(): %d\n", fsMount(argv[1], atoi(argv[2]), "warren"));

    char *fname = "warren/urandom.txt";

    printf("Opening warren/urandom.txt\n");
    int ff = fsOpen(fname, 1);

    printf("Closed warren/urandom.txt\n");
    fsClose(ff);

    printf("Remove(%s): %d\n", fname, fsRemove(fname));

    return 0;
}
