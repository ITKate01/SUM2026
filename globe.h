#ifndef _GLOBE_H
#define _GLOBE_H

#include <windows.h>
#include <math.h>

#define GLB_GRID_W 31
#define GLB_GRID_H 18

typedef struct
{
DOUBLE X, Y, Z;
} VEC;

VOID GLB_Init(DOUBLE R);
VOID GLB_Resize(INT W, INT H);
VOID GLB_Draw(HDC hDC);

#endif

