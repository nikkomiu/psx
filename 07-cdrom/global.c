#include "global.h"

#include <libgte.h>
#include <libgpu.h>

static u_long ot[2][OT_LENGTH];
static char primbuff[2][PB_LENGTH];
static char* nextprim;

void EmptyOT(u_short curbuff)
{
    ClearOTagR(ot[curbuff], OT_LENGTH);
}

void SetOTAt(u_short curbuff, u_int i, u_long value)
{
    ot[curbuff][i] = value;
}

u_long* GetOTAt(u_short curbuff, u_int i)
{
    return &ot[curbuff][i];
}

void IncrementNextPrim(u_int size)
{
    nextprim += size;
}

void SetNextPrim(char* value)
{
    nextprim = value;
}

char* GetNextPrim(void)
{
    return nextprim;
}

void ResetNextPrim(u_short curbuff)
{
    nextprim = primbuff[curbuff];
}
