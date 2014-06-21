#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main (void) {

    DIR *dp;
    struct dirent *ep;

    // Open the working directory
    dp = opendir("./");

    if (dp != NULL) {
        while ((ep = readdir(dp))) {
            printf("%s ", ep->d_name);
            printf("%d\n", ep->d_namlen);
        }
        (void) closedir(dp);
    } else {
      puts("Couldn't open the directory.");
    }

    return 0;
}
