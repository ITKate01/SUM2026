#include "globe.h"
#include <math.h>

typedef DOUBLE DBL;
static VEC GLB_Geom[GLB_GRID_H][GLB_GRID_W];
static POINT pnts[GLB_GRID_H][GLB_GRID_W];
static INT GLB_Ws = 800, GLB_Hs = 600;
static DBL /*GLB_ProjectDist,*/ ProjectSize = 1, GLB_Wp, GLB_Hp;
#define PI 3.14159265358979323846

VOID GLB_Init(DBL R)
{
  INT i, j;
  DBL theta, phi;
  
  for (i = 0; i < GLB_GRID_H; i++)
  {
    theta = i * PI / (GLB_GRID_H - 1);
    for (j = 0; j < GLB_GRID_W; j++)
    {
        phi = j * 2 * PI / (GLB_GRID_W - 1);

        GLB_Geom[i][j].X = R * sin(theta) * sin(phi);
        GLB_Geom[i][j].Y = R * cos(theta);
        GLB_Geom[i][j].Z = R * sin(theta) * cos(phi);
    }
  }
}

VEC RotateZ( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
 
  NewP.X = P.X * co - P.Y * si;
  NewP.Y = P.X * si + P.Y * co;
  NewP.Z = P.Z;
  return NewP;
}

VEC RotateX( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
 
  NewP.X = P.X;
  NewP.Y = P.Y * co - P.Z * si;
  NewP.Z = P.Y * si + P.Z * co;
  return NewP;
}

VEC RotateY( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
 
  NewP.X = P.Z * si + P.X * co;
  NewP.Y = P.Y;
  NewP.Z = P.Z * co - P.X * si;
  return NewP;
}

VOID GLB_Resize(INT W, INT H)
{
  GLB_Ws = W;
  GLB_Hs = H;

  
  if(GLB_Ws >= GLB_Hs)
  {
    GLB_Wp = ProjectSize * GLB_Ws / GLB_Hs;
    GLB_Hp = ProjectSize;
  }
  else
  {
    GLB_Wp = ProjectSize;
    GLB_Hp = ProjectSize * GLB_Hs / GLB_Ws;
  }
}

VOID GLB_Draw(HDC hDC)
{
  INT i, j;
  INT s = 2;
  static POINT pnts[GLB_GRID_H][GLB_GRID_W]; 



  for (i = 0; i < GLB_GRID_H; i++)
  {
    for (j = 0; j < GLB_GRID_W; j++)
    {
        pnts[i][j].x = (INT)(GLB_Ws / 2 + GLB_Geom[i][j].X * (GLB_Ws / GLB_Wp + GLB_Ws / 2));
        pnts[i][j].y = (INT)(GLB_Hs / 2 - GLB_Geom[i][j].Y * (GLB_Hs / GLB_Hp + GLB_Hs /2));
    }
  }

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 255));
  SetDCBrushColor(hDC, RGB(0, 0, 255));
  for (i = 0; i < GLB_GRID_H; i++)
  {
    for (j = 0; j < GLB_GRID_W; j++)
    {
        Ellipse(hDC, pnts[i][j].x - s, pnts[i][j].y - s,
                     pnts[i][j].x + s, pnts[i][j].y + s);
    }
  }

  for(i = 0; i < GLB_GRID_H; i++)
  {
    MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
    for(j = 1; j < GLB_GRID_W; j++)
    {
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
    }
  }
  for(i = 1; i < GLB_GRID_W; i++)
  {
    MoveToEx(hDC, pnts[0][i].x, pnts[0][i].y, NULL);
    for(j = 0; j < GLB_GRID_H; j++)
    {
      LineTo(hDC, pnts[j][i].x, pnts[j][i].y);
    }
  }
}

