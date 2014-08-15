#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ece454_fs.h"

int main(int argc, char *argv[]) {
    char *dirname = NULL;
   printf("Running first test case.\n");
    char *srvIpOrDomName = argv[2];
	int srvPort = atoi(argv[3]);
    if(argc > 1) dirname = argv[1];
    else {
	dirname = (char *)calloc(strlen(".")+1, sizeof(char));
	strcpy(dirname, ".");
    }

    printf("fsMount(): %d\n", fsMount(srvIpOrDomName, srvPort, dirname));
    FSDIR *fd = fsOpenDir(dirname);
    if(fd == NULL) {
	perror("fsOpenDir"); exit(1);
    }

    struct fsDirent *fdent = NULL;
    for(fdent = fsReadDir(fd); fdent != NULL; fdent = fsReadDir(fd)) {
	printf("\t %s, %d\n", fdent->entName, (int)(fdent->entType));
    }

    if(errno != 0) {
	perror("fsReadDir");
    }

    printf("fsCloseDir(): %d\n", fsCloseDir(fd));

char t1_file_name[] = "/test_file1";
	char* path = malloc(strlen(dirname)+strlen(t1_file_name)+1);
*path = '\0';
strcat(path, dirname);
strcat(path, t1_file_name);

    int ff = fsOpen(path, 0);
    if(ff < 0) {
	perror("fsOpen"); exit(1);
    }
    else printf("fsOpen(): %d\n", ff);

    char data[11];
    if(fsRead(ff, (void *)data, 10) < 0) {
	perror("fsRead"); exit(1);
    }
data[10] = '\0';
if (strcmp(data,"hahahahaha")) { perror("Read does not work correctly"); exit(1); }


    printf("fsClose(): %d\n", fsClose(ff));

	printf("%s\n", path);
    printf("fsRemove(%s): %d\n", path, fsRemove(path));

(*path) = '\0';
strcat(path, dirname);
strcat(path, "/test_file2");

    ff = fsOpen(path, 1);
    if(ff < 0) {
	perror("fsOpen(write)"); exit(1);
    }

char buf[] = "Write stuff...";
    if(fsWrite(ff, buf, strlen(buf)) < strlen(buf)) {
	fprintf(stderr, "fsWrite() wrote fewer than 256\n");
    }

    if(fsClose(ff) < 0) {
	perror("fsClose"); exit(1);
    }


   printf("Finished running first test case.\n");

    return 0;
}
