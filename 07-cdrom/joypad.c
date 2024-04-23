#include "joypad.h"

static u_long padstate = 0;

void JoyPadInit(void)
{
    PadInit(0);
}

void JoyPadReset(void)
{
    padstate = 0;
}

void JoyPadUpdate(void)
{
    u_long ps = PadRead(0);
    padstate = ps;
}

int JoyPadCheck(int p)
{
    return (padstate & p);
}
