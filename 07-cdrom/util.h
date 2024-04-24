#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys/types.h>

#include "global.h"

char* FileRead(char* filename, u_long* length);

char GetChar(u_char* bytes, u_long* offset);
short GetShortBE(u_char* bytes, u_long* offset);
short GetShortLE(u_char* bytes, u_long* offset);

#endif
