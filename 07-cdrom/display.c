#include "display.h"

#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>

#include "global.h"

typedef struct
{
    DRAWENV draw[2];
    DISPENV disp[2];
} DoubleBuff;

static DoubleBuff screen;
static u_short curbuff;

void DisplayInit(void)
{
    // Reset GPU
    ResetGraph(VIDEO_MODE);

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
    DrawOTag(GetOTAt(curbuff, OT_LENGTH - 1));

    // Swap current buffer
    curbuff = !curbuff;

    // Reset the pointer to the next primitive
    ResetNextPrim(curbuff);
}

u_short GetCurBuff()
{
    return curbuff;
}
