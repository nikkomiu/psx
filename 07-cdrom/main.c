#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

#include "global.h"
#include "util.h"
#include "object.h"
#include "display.h"
#include "joypad.h"
#include "camera.h"

extern char __heap_start, __sp;

Camera camera;
Object object;

typedef struct
{
    SVECTOR rot;
    VECTOR  pos;
    VECTOR  scale;

    VECTOR vel;
    VECTOR acc;

    SVECTOR verts[8];
    short faces[24];
} Cube;

char primbuff[2][PB_LENGTH];

POLY_F4* polyf4;
POLY_G4* polyg4;
POLY_G3* polyg3;

MATRIX worldmat = {0};
MATRIX viewmat = {0};

Object object;

void Setup(void)
{
    InitHeap3((u_long*)&__heap_start, &__sp - 0x5000 - &__heap_start);

    DisplayInit();

    CdInit();

    JoyPadInit();

    // Reset the pointer to the next primitive
    ResetNextPrim(GetCurBuff());

    setVector(&camera.position, 500, -1000, -1500);
    camera.lookat = (MATRIX){0};

    LoadObjectData(&object, "MODEL");
}

void Update(void)
{
    int i, q, nclip;
    long otz, p, flag;

    // Clear the ordering table
    EmptyOT(GetCurBuff());

    JoyPadUpdate();

    if (JoyPadCheck(PAD1_L1))
    {
        object.rotation.vy -= 20;
    }
    if (JoyPadCheck(PAD1_R1))
    {
        object.rotation.vy += 20;
    }
    if (JoyPadCheck(PAD1_LEFT))
    {
        camera.position.vx -= 50;
    }
    if (JoyPadCheck(PAD1_RIGHT))
    {
        camera.position.vx += 50;
    }
    if (JoyPadCheck(PAD1_UP))
    {
        camera.position.vy -= 50;
    }
    if (JoyPadCheck(PAD1_DOWN))
    {
        camera.position.vy += 50;
    }
    if (JoyPadCheck(PAD1_CROSS))
    {
        camera.position.vz += 50;
    }
    if (JoyPadCheck(PAD1_CIRCLE))
    {
        camera.position.vz -= 50;
    }

    LookAt(&camera, &camera.position, &object.position, &(VECTOR){0, -ONE, 0});

    ////////////////////////////////////////////
    // Draw the Object                        //
    ////////////////////////////////////////////
    RotMatrix(&object.rotation, &worldmat);     // Populate the world matrix with the current rotation values
    TransMatrix(&worldmat, &object.position);   // Populate the world matrix with the current translation values
    ScaleMatrix(&worldmat, &object.scale);      // Populate the world matrix with the current scale values

    // Create the view matrix by combining the world matrix and lookat matrix
    CompMatrixLV(&camera.lookat, &worldmat, &viewmat);

    SetRotMatrix(&viewmat);   // Sets the rotation matrix to be used by the GTE (RotTransPers)
    SetTransMatrix(&viewmat); // Sets the translation matrix to be used by the GTE (RotTransPers)

    printf("Obj: Pos: %d, %d, %d\n", object.position.vx, object.position.vy, object.position.vz);
    printf("Obj: Rot: %d, %d, %d\n", object.rotation.vx, object.rotation.vy, object.rotation.vz);
    printf("Obj: Faces: %d\n", object.numfaces);
    printf("Obj: Verts: %d\n", object.numverts);
    printf("Obj: Colors: %d\n", object.numcolors);

    // for each vertex of each face on the cube
    for (i = 0, q = 0; i < object.numfaces; i += 4, q++)
    {
        polyf4 = (POLY_F4*)GetNextPrim();
        setPolyF4(polyf4);
        setRGB0(polyf4, object.colors[q].r, object.colors[q].g, object.colors[q].b);

        nclip = RotAverageNclip4(
            &object.verts[object.faces[i]],
            &object.verts[object.faces[i+1]],
            &object.verts[object.faces[i+2]],
            &object.verts[object.faces[i+3]],
            (long*)&polyf4->x0,
            (long*)&polyf4->x1,
            (long*)&polyf4->x2,
            (long*)&polyf4->x3,
            &p, &otz, &flag
        );
        if (nclip <= 0)
        {
            continue;
        }

        if (otz > 0 && otz < OT_LENGTH)
        {
            addPrim(GetOTAt(GetCurBuff(), otz), polyf4);
            IncrementNextPrim(sizeof(POLY_F4));
        }
    }
}

void Render(void)
{
    DisplayFrame();
}

int main(void)
{
    Setup();

    while (1)
    {
        Update();
        Render();
    }

    return 0;
}
