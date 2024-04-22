#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <sys/types.h>

#define VIDEO_MODE 0
#define SCREEN_RES_X 320
#define SCREEN_RES_Y 240
#define SCREEN_CENTER_X (SCREEN_RES_X >> 1)
#define SCREEN_CENTER_Y (SCREEN_RES_Y >> 1)
#define SCREEN_Z 320

void DisplayInit(void);
void DisplayFrame(void);

u_short GetCurBuff();

#endif
