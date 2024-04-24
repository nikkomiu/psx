#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <sys/types.h>
#include <libgte.h>

#define OBJECT_FILENAME_LENGTH 16

typedef struct
{
    SVECTOR rotation;
    VECTOR position;
    VECTOR scale;

    short numverts;
    SVECTOR* verts;

    short numfaces;
    short* faces;

    short numcolors;
    CVECTOR* colors;
} Object;

void LoadObjectData(Object* obj, char* filename);

#endif
