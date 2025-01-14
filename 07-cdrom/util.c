#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <libcd.h>

char* FileRead(char* filename, u_long* length)
{
    CdlFILE filepos;
    int numsectors;
    char* buffer = NULL;

    // Search for file
    if (CdSearchFile(&filepos, filename) == NULL) {
        printf("File not found: %s\n", filename);
        return NULL;
    }

    // Allocate buffer for file (size + 2047) / 2048
    numsectors = filepos.size + 2047 >> 11;
    buffer = (char*)malloc3(numsectors << 11);
    if (buffer == NULL) {
        printf("Failed to allocate buffer of size %d for file: %s\n", numsectors, filename);
        return NULL;
    }

    // Read file into buffer
    CdControl(CdlSetloc, (u_char*)&filepos.pos, 0);
    CdRead(numsectors, (u_long*)buffer, CdlModeSpeed);
    CdReadSync(0, 0);

    // Set length and return buffer
    *length = filepos.size;
    return buffer;
}

char GetChar(u_char* bytes, u_long* offset)
{
    return bytes[(*offset)++];
}

short GetShortLE(u_char* bytes, u_long* offset)
{
    return bytes[(*offset)++] | (bytes[(*offset)++] << 8);
}

short GetShortBE(u_char* bytes, u_long* offset)
{
    return (bytes[(*offset)++] << 8) | bytes[(*offset)++];
}

long GetLongLE(u_char* bytes, u_long* offset)
{
    return bytes[(*offset)++] | (bytes[(*offset)++] << 8) | (bytes[(*offset)++] << 16) | (bytes[(*offset)++] << 24);
}

long GetLongBE(u_char* bytes, u_long* offset)
{
    return (bytes[(*offset)++] << 24) | (bytes[(*offset)++] << 16) | (bytes[(*offset)++] << 8) | bytes[(*offset)++];
}
