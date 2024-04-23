#include "camera.h"

#include <libgpu.h>

void LookAt(Camera* camera, VECTOR* eye, VECTOR* target, VECTOR* up)
{
    VECTOR xright, yup, zforward;
    VECTOR x, y, z;
    VECTOR pos, t;

    zforward.vx = target->vx - eye->vx;
    zforward.vy = target->vy - eye->vy;
    zforward.vz = target->vz - eye->vz;
    VectorNormal(&zforward, &z);

    OuterProduct12(&z, up, &xright);
    VectorNormal(&xright, &x);

    OuterProduct12(&z, &x, &yup);
    VectorNormal(&yup, &y);

    camera->lookat.m[0][0] = x.vx; camera->lookat.m[0][1] = x.vy; camera->lookat.m[0][2] = x.vz;
    camera->lookat.m[1][0] = y.vx; camera->lookat.m[1][1] = y.vy; camera->lookat.m[1][2] = y.vz;
    camera->lookat.m[2][0] = z.vx; camera->lookat.m[2][1] = z.vy; camera->lookat.m[2][2] = z.vz;

    pos.vx = -eye->vx;
    pos.vy = -eye->vy;
    pos.vz = -eye->vz;

    ApplyMatrixLV(&camera->lookat, &pos, &t);
    TransMatrix(&camera->lookat, &t);
}
