#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <sys/types.h>
#include <libgte.h>

#define OBJECT_FILENAME_LENGTH 16

typedef struct
{
    short numverts;
    SVECTOR* verts;

    short numfaces;
    short* faces;

    short numcolors;
    CVECTOR* colors;
} Object;

Object LoadObject(char* filename);

#endif
