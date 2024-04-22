#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <sys/types.h>

#define OT_LENGTH 8192
#define PB_LENGTH 131072

void EmptyOT(u_short curbuff);

void SetOTAt(u_short curbuff, u_int i, u_long value);
u_long* GetOTAt(u_short curbuff, u_int i);

void IncrementNextPrim(u_int size);
void SetNextPrim(char* value);
char* GetNextPrim(void);
void ResetNextPrim(u_short curbuff);

#endif
