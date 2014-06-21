/*
 * Mahesh V. Tripunitara
 * University of Waterloo
 * You specify what goes in this file. I just have a "dummy"
 * specification of the FSDIR type.
 */

#ifndef _ECE_FS_OTHER_INCLUDES_
#define _ECE_FS_OTHER_INCLUDES_
#include <sys/types.h>
#include <dirent.h>

typedef DIR FSDIR;

struct fsDirent {
    char entName[256];
    unsigned char entType; /* 0 for file, 1 for folder,
			      -1 otherwise. */
};

#endif
