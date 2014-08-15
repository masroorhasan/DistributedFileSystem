#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ece454_fs.h"

int main(int argc, char *argv[]) {
   printf("Running third test case.\n");
    char *dirname = argv[1];
    
    char *srvIpOrDomName = argv[2];
	int srvPort = atoi(argv[3]);
	int id = atoi(argv[4]);
printf("here");
    fsMount(srvIpOrDomName, srvPort, dirname);
printf("here");
char t1_file_name[] = "/test_file1";
	char* path = malloc(strlen(dirname)+strlen(t1_file_name)+1);
*path = '\0';
strcat(path, dirname);
strcat(path, t1_file_name);

char buf[] = "abcdef123456";

int i;
for (i = 1; i <= 20; i++) {
printf("%d: %d\n", id, i);
sleep(1);
    int ff = fsOpen(path, 1);
    if(ff < 0) {
	perror("fsOpen(write)"); exit(1);
    }


    if(fsWrite(ff, buf + (id-1)*6, 3) < 3) {
//	fprintf(stderr, "fsWrite() wrote fewer than 3\n");
    }

if(fsWrite(ff, buf + (id-1)*6 + 3, 3) < 3) {
//	fprintf(stderr, "fsWrite() wrote fewer than 3\n");
    }

    if(fsClose(ff) < 0) {
	perror("fsClose"); exit(1);
    }

}

if (id == 1) {
    int ff = fsOpen(path, 0);
    if(ff < 0) {
	perror("fsOpen"); exit(1);
    }
//    else printf("fsOpen(): %d\n", ff);

    char data[7];
    if(fsRead(ff, (void *)data, 6) < 0) {
	perror("fsRead"); exit(1);
    }

data[6] = '\0';

if (strcmp(data,"abcdef") && strcmp(data, "123456")) { printf("Something is wrong... Investigate"); fsClose(ff); exit(1); }
else { printf("Give full mark"); fsClose(ff); exit(0); }

    fsClose(ff);

}
   printf("Finsihed running third test case.\n");
    return 0;
}
