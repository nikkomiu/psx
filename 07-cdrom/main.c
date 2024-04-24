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

POLY_G4* poly4;
POLY_G3* poly3;

MATRIX worldmat = {0};
MATRIX viewmat = {0};

Cube cube0 =
{
    .rot = {0, 0, 0},
    .pos = {0, -400, 2000},
    .scale = {ONE, ONE, ONE},

    .vel = {0, 0, 0},
    .acc = {0, 1, 0},

    .verts = {
        { -128, -128, -128 },
        {  128, -128, -128 },
        {  128, -128,  128 },
        { -128, -128,  128 },
        { -128,  128, -128 },
        {  128,  128, -128 },
        {  128,  128,  128 },
        { -128,  128,  128 }
    },
    .faces = {
        3, 2, 0, 1, // top
        0, 1, 4, 5, // front
        4, 5, 7, 6, // bottom
        1, 2, 5, 6, // right
        2, 3, 6, 7, // back
        3, 0, 7, 4, // left
    }
};

void Setup(void)
{
    InitHeap3((u_long*)&__heap_start, &__sp - 0x5000 - &__heap_start);

    DisplayInit();

    CdInit();

    JoyPadInit();

    // Reset the pointer to the next primitive
    ResetNextPrim(GetCurBuff());

    camera.position.vx = 500;
    camera.position.vy = -1000;
    camera.position.vz = -1500;
    camera.lookat = (MATRIX){0};

    LoadObject("MODEL");
}

void Update(void)
{
    int i, nclip;
    long otz, p, flag;

    // Clear the ordering table
    EmptyOT(GetCurBuff());

    JoyPadUpdate();

    if (JoyPadCheck(PAD1_L1))
    {
        cube0.rot.vy -= 20;
    }
    if (JoyPadCheck(PAD1_R1))
    {
        cube0.rot.vy += 20;
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

    LookAt(&camera, &camera.position, &cube0.pos, &(VECTOR){0, -ONE, 0});

    ////////////////////////////////////////////
    // Draw the Cube                          //
    ////////////////////////////////////////////
    RotMatrix(&cube0.rot, &worldmat);     // Populate the world matrix with the current rotation values
    TransMatrix(&worldmat, &cube0.pos);   // Populate the world matrix with the current translation values
    ScaleMatrix(&worldmat, &cube0.scale); // Populate the world matrix with the current scale values

    // Create the view matrix by combining the world matrix and lookat matrix
    CompMatrixLV(&camera.lookat, &worldmat, &viewmat);

    SetRotMatrix(&viewmat);               // Sets the rotation matrix to be used by the GTE (RotTransPers)
    SetTransMatrix(&viewmat);             // Sets the translation matrix to be used by the GTE (RotTransPers)

    // for each vertex of each face on the cube
    for (i = 0; i < 6 * 4; i += 4)
    {
        poly4 = (POLY_G4*)GetNextPrim();
        setPolyG4(poly4);
        setRGB0(poly4, 255,   0, 255);
        setRGB1(poly4, 255, 255,   0);
        setRGB2(poly4,   0, 255, 255);
        setRGB3(poly4,   0, 255,   0);

        nclip = RotAverageNclip4(
            &cube0.verts[cube0.faces[i]],
            &cube0.verts[cube0.faces[i+1]],
            &cube0.verts[cube0.faces[i+2]],
            &cube0.verts[cube0.faces[i+3]],
            (long*)&poly4->x0,
            (long*)&poly4->x1,
            (long*)&poly4->x2,
            (long*)&poly4->x3,
            &p, &otz, &flag
        );
        if (nclip <= 0)
        {
            continue;
        }

        if (otz > 0 && otz < OT_LENGTH)
        {
            addPrim(GetOTAt(GetCurBuff(), otz), poly4);
            IncrementNextPrim(sizeof(POLY_G4));
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
