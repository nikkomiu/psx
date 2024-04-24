#include "object.h"

#include <stdio.h>
#include "util.h"

Object LoadObject(char* filename)
{
    char filepath[OBJECT_FILENAME_LENGTH];
    sprintf(filepath, "\\%s.BIN;1", filename);

    Object obj;
    char* bytes;
    u_long length, parseb = 0;
    bytes = FileRead(filepath, &length);
    printf("Read %ld bytes for %s\n", length, filepath);

#if 0
    for (int i = 0; i < length; i++)
    {
        printf("%08X ", bytes[i]);
        if (i % 8 == 7)
        {
            printf("\n");
        }
    }
    printf("\n");
#endif

    obj.numverts = GetShortBE(bytes, &parseb);
    printf("VERTS: %d\n", obj.numverts);
    obj.verts = malloc(obj.numverts * sizeof(SVECTOR));
    for (int i = 0; i < obj.numverts; i++)
    {
        obj.verts[i].vx = GetShortBE(bytes, &parseb);
        obj.verts[i].vy = GetShortBE(bytes, &parseb);
        obj.verts[i].vz = GetShortBE(bytes, &parseb);
        printf("Vertex %02d: X=%05d, Y=%05d, Z=%05d\n", i, obj.verts[i].vx, obj.verts[i].vy, obj.verts[i].vz);
    }

    obj.numfaces = GetShortBE(bytes, &parseb);
    printf("FACES: %d\n", obj.numfaces);
    obj.faces = malloc(obj.numfaces * sizeof(short));
    for (int i = 0; i < obj.numfaces; i++)
    {
        obj.faces[i] = GetShortBE(bytes, &parseb);
        printf("Face %02d: %02d\n", i, obj.faces[i]);
    }

    obj.numcolors = GetChar(bytes, &parseb);
    printf("COLORS: %d\n", obj.numcolors);
    obj.colors = malloc(obj.numcolors * sizeof(CVECTOR));
    for (int i = 0; i < obj.numcolors; i++)
    {
        obj.colors[i].r = GetChar(bytes, &parseb);
        obj.colors[i].g = GetChar(bytes, &parseb);
        obj.colors[i].b = GetChar(bytes, &parseb);
        obj.colors[i].cd = GetChar(bytes, &parseb);
        printf("Color: R=%02d, G=%02d, B=%02d, CD=%02d\n");
    }

    free(bytes);
}
