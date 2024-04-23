#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <sys/types.h>
#include <libgte.h>

typedef struct
{
    VECTOR position;
    SVECTOR rotation;
    MATRIX lookat;
} Camera;

void LookAt(Camera* camera, VECTOR* eye, VECTOR* target, VECTOR* up);

#endif
