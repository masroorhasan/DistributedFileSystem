#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ece454_fs.h"

int main(int argc, char *argv[]) {
    char *dirname1 = argv[1];    
    char *srvIpOrDomName1 = argv[2];
    int srvPort1 = atoi(argv[3]); 
    char *dirname2 = argv[4];
    char *srvIpOrDomName2 = argv[5];
    int srvPort2 = atoi(argv[6]);
    int id = atoi(argv[7]);

   printf("Running second test case.\n");

    printf("fsMount(): %d\n", fsMount(srvIpOrDomName1, srvPort1, dirname1));

    printf("fsMount(): %d\n", fsMount(srvIpOrDomName2, srvPort2, dirname2));

    FSDIR *fd = fsOpenDir(dirname1);
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

    fd = fsOpenDir(dirname2);
    if(fd == NULL) {
	perror("fsOpenDir"); exit(1);
    }

    for(fdent = fsReadDir(fd); fdent != NULL; fdent = fsReadDir(fd)) {
	printf("\t %s, %d\n", fdent->entName, (int)(fdent->entType));
    }

    if(errno != 0) {
	perror("fsReadDir");
    }

    printf("fsCloseDir(): %d\n", fsCloseDir(fd));


char t1_file_name[] = "/test_file1";
	char* path = malloc(strlen(dirname1)+strlen(t1_file_name)+1);
*path = '\0';
strcat(path, dirname1);
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
strcat(path, dirname1);
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


/////////////////////////////////////////////////////////////////////
if (id != 3)
{
char t1_file_name[] = "/test_file2";
	char* path = malloc(strlen(dirname2)+strlen(t1_file_name)+1);
*path = '\0';
strcat(path, dirname2);
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

if (strcmp(data,"Write stuf")) { printf("Write does not work correctly\n"); exit(1); }

    printf("fsClose(): %d\n", fsClose(ff));

	printf("%s\n", path);
    printf("fsRemove(%s): %d\n", path, fsRemove(path));

(*path) = '\0';
strcat(path, dirname2);
strcat(path, "/test_file3");

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
}
   printf("Finsihed running second test case.\n");
//////////////////////////////////////////////////////////////
    return 0;
}
