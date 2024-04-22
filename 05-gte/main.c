#include <stdlib.h>
#include <sys/types.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>

#define VIDEO_MODE 0
#define SCREEN_RES_X 320
#define SCREEN_RES_Y 240
#define SCREEN_CENTER_X (SCREEN_RES_X >> 1)
#define SCREEN_CENTER_Y (SCREEN_RES_Y >> 1)
#define SCREEN_Z 320

#define OT_LENGTH 2048
#define PB_LENGTH 2048

#define NUM_VERTS 8
#define NUM_FACES 6

SVECTOR verts[] =
{
    { -128, -128, -128 },
    {  128, -128, -128 },
    {  128, -128,  128 },
    { -128, -128,  128 },
    { -128,  128, -128 },
    {  128,  128, -128 },
    {  128,  128,  128 },
    { -128,  128,  128 }
};

short faces[] =
{
    3, 2, 0, 1, // top
    0, 1, 4, 5, // front
    4, 5, 7, 6, // bottom
    1, 2, 5, 6, // right
    2, 3, 6, 7, // back
    3, 0, 7, 4, // left
};

typedef struct
{
    DRAWENV draw[2];
    DISPENV disp[2];
} DoubleBuff;

DoubleBuff screen;
u_short curbuff;

u_long ot[2][OT_LENGTH];

char primbuff[2][PB_LENGTH];
char *nextprim;

POLY_G4 *poly4;
POLY_G3 *poly3;

SVECTOR rotation = {0, 0, 0};
VECTOR translation = {0, 0, 900};
VECTOR scale = {ONE, ONE, ONE};

MATRIX world = {0};

VECTOR vel = {0, 0, 0};
VECTOR acc = {0, 0, 0};
VECTOR pos = {0, 0, 0};

void ScreenInit(void)
{
    // Reset GPU
    ResetGraph(0);

    // Set the display area of the first buffer
    SetDefDispEnv(&screen.disp[0], 0,   0, SCREEN_RES_X, SCREEN_RES_Y);
    SetDefDrawEnv(&screen.draw[0], 0, 240, SCREEN_RES_X, SCREEN_RES_Y);

    // Set the display area of the second buffer
    SetDefDispEnv(&screen.disp[1], 0, 240, SCREEN_RES_X, SCREEN_RES_Y);
    SetDefDrawEnv(&screen.draw[1], 0,   0, SCREEN_RES_X, SCREEN_RES_Y);

    // Set the back/drawing buffer
    screen.draw[0].isbg = 1;
    screen.draw[1].isbg = 1;

    // Set the background clear color
    setRGB0(&screen.draw[0], 63, 0, 127);
    setRGB0(&screen.draw[1], 63, 0, 127);

    // Set the current initial buffer
    curbuff = 0;
    PutDispEnv(&screen.disp[curbuff]);
    PutDrawEnv(&screen.draw[curbuff]);

    // Initialize and set up the GTE geometry offsets
    InitGeom();
    SetGeomOffset(SCREEN_CENTER_X, SCREEN_CENTER_Y);
    SetGeomScreen(SCREEN_Z);

    // Enable the display
    SetDispMask(1);
}

void DisplayFrame(void)
{
    DrawSync(0);
    VSync(0);

    PutDispEnv(&screen.disp[curbuff]);
    PutDrawEnv(&screen.draw[curbuff]);

    // Draw the ordering table
    DrawOTag(ot[curbuff] + OT_LENGTH - 1);

    // Swap current buffer
    curbuff = !curbuff;

    // Reset the pointer to the next primitive
    nextprim = primbuff[curbuff];
}

void Setup(void)
{
    ScreenInit();

    // Reset the pointer to the next primitive
    nextprim = primbuff[curbuff];

    acc.vy = 1;
    pos.vy = -400;
    pos.vz = 1800;
}

void Update(void)
{
    int i, nclip;
    long otz, p, flag;

    // update position based on acc and vel
    vel.vx += acc.vx;
    vel.vy += acc.vy;
    vel.vz += acc.vz;

    pos.vx += vel.vx >> 1;
    pos.vy += vel.vy >> 1;
    pos.vz += vel.vz >> 1;

    if (pos.vy > 400)
    {
        vel.vy *= -1;
    }

    // Clear the ordering table
    ClearOTagR(ot[curbuff], OT_LENGTH);

    RotMatrix(&rotation, &world);      // Populate the world matrix with the current rotation values
    TransMatrix(&world, &pos);         // Populate the world matrix with the current translation values
    ScaleMatrix(&world, &scale);       // Populate the world matrix with the current scale values

    SetRotMatrix(&world);              // Sets the rotation matrix to be used by the GTE (RotTransPers)
    SetTransMatrix(&world);            // Sets the translation matrix to be used by the GTE (RotTransPers)

    for (i = 0; i < NUM_FACES * 4; i += 4)
    {
        poly4 = (POLY_G4*)nextprim;
        setPolyG4(poly4);
        setRGB0(poly4, 255,   0, 255);
        setRGB1(poly4, 255, 255,   0);
        setRGB2(poly4,   0, 255, 255);
        setRGB3(poly4,   0, 255,   0);

        nclip = RotAverageNclip4(
            &verts[faces[i]],
            &verts[faces[i+1]],
            &verts[faces[i+2]],
            &verts[faces[i+3]],
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
            addPrim(ot[curbuff][otz], poly4);
            nextprim += sizeof(POLY_G4);
        }
    }

    // for (i = 0; i < NUM_FLOOR_FACES * 3; i += 3)
    // {
    //     poly3 = (POLY_G3*)nextprim;
    //     setPolyG3(poly3);
    //     setRGB0(poly3, 255, 255, 0);
    //     setRGB1(poly3, 255, 0, 255);
    //     setRGB2(poly3, 0, 255, 255);

    //     nclip = RotAverageNclip3(
    //     );
    // }

    rotation.vx += 6;
    rotation.vy += 8;
    rotation.vz += 12;
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
