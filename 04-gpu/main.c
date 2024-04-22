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
#define SCREEN_Z 400

#define OT_LENGTH 16
#define PB_LENGTH 2048

typedef struct
{
    DRAWENV draw[2];
    DISPENV disp[2];
} DoubleBuff;

DoubleBuff screen;
u_short curbuff;

u_long ot[2][OT_LENGTH];

char primbuff[2][PB_LENGTH];
char* nextprim;

POLY_F3* tria0;
TILE*    tile0;
POLY_G4* quad0;

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
}

void Update(void)
{
    // Clear the ordering table
    ClearOTagR(ot[curbuff], OT_LENGTH);

    tile0 = (TILE*)nextprim;                          // Create next primitive
    setTile(tile0);                                    // Initialize the primitive
    setXY0(tile0, 82, 32);                             // Set the primitive (x, y) position
    setWH(tile0, 64, 64);                              // Set the primitive (w, h) size
    setRGB0(tile0, 0, 255, 0);                         // Set the primitive color
    addPrim(ot[curbuff], tile0);                       // Add the primitive to the ordering table
    nextprim += sizeof(TILE);                          // Move the pointer to the next primitive

    tria0 = (POLY_F3*)nextprim;                       // Create next primitive
    setPolyF3(tria0);                                  // Initialize the primitive
    setXY3(tria0, 64, 100, 200, 150, 50, 220);         // Set the primitive (x1, y1, x2, y2, x3, y3) position
    setRGB0(tria0, 255, 0, 255);                       // Set the primitive color
    addPrim(ot[curbuff], tria0);                       // Add the primitive to the ordering table
    nextprim += sizeof(POLY_F3);                       // Move the pointer to the next primitive

    quad0 = (POLY_G4*)nextprim;                       // Create next primitive
    setPolyG4(quad0);                                  // Initialize the primitive
    setXY4(quad0, 200, 20, 250, 35, 180, 50, 240, 80); // Set the primitive (x1, y1, x2, y2, x3, y3, x4, y4) position
    setRGB0(quad0, 255, 255, 0);                       // Set v0 color
    setRGB1(quad0, 0, 255, 255);                       // Set v1 color
    setRGB2(quad0, 255, 0, 255);                       // Set v2 color
    setRGB3(quad0, 0, 0, 255);                         // Set v3 color
    addPrim(ot[curbuff], quad0);                       // Add the primitive to the ordering table
    nextprim += sizeof(POLY_G4);                       // Move the pointer to the next position
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
