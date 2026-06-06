#include <windows.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846
#define WND_CLASS_NAME "My CGSG SummerPractice'2026 Clock Window"

LRESULT CALLBACK MyWindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

VOID FlipFullScreen(HWND hWnd)
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;
  
  if (!IsFullScreen) 
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;

    GetWindowRect(hWnd, &SaveRect);

    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hmon, &mi);

    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);

    IsFullScreen = TRUE;  
  }
  else  
  {
    SetWindowPos(hWnd, HWND_TOP, SaveRect.left, SaveRect.top, SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top, SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;  
  }
}

VOID DrawHand(HDC hDC, INT Cx, INT Cy, INT Len, INT Width, DOUBLE angle, INT r, INT g, INT b)
{
  INT x2, y2;
  HPEN hPen, hOldPen;
  HBRUSH hBrush, hOldBrush;
  POINT pts[3];
  
  x2 = Cx + (INT)(Len * sin(angle));
  y2 = Cy - (INT)(Len * cos(angle));
  
  hPen = CreatePen(PS_SOLID, Width, RGB(r, g, b));
  hBrush = CreateSolidBrush(RGB(r, g, b));
  
  hOldPen = SelectObject(hDC, hPen);
  hOldBrush = SelectObject(hDC, hBrush);
  
  MoveToEx(hDC, Cx, Cy, NULL);
  LineTo(hDC, x2, y2); 
  
  pts[0].x = x2;
  pts[0].y = y2;
  pts[1].x = x2 - (INT)(8 * cos(angle)) - (INT)(4 * sin(angle));
  pts[1].y = y2 - (INT)(8 * sin(angle)) + (INT)(4 * cos(angle));
  pts[2].x = x2 - (INT)(8 * cos(angle)) + (INT)(4 * sin(angle));
  pts[2].y = y2 - (INT)(8 * sin(angle)) - (INT)(4 * cos(angle));
  
 /* Polygon(hDC, pts, 3); */ 
  
  SelectObject(hDC, hOldPen);
  SelectObject(hDC, hOldBrush);
  
  DeleteObject(hPen);
  DeleteObject(hBrush);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow)
{
  HWND hWnd;
  WNDCLASS wc;
  MSG msg;

  wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;  
  wc.cbClsExtra = 0;
  wc.cbWndExtra = sizeof(void *) * 4;  
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hInstance = hInstance;
  wc.lpszMenuName = NULL; 
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpfnWndProc = MyWindowFunc;  

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_ICONERROR | MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "Clock",
                      WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
                      100, 100, 800, 600, NULL, NULL, hInstance, NULL);

  if (hWnd == NULL)
  {
    MessageBox(NULL, "Error create window", "ERROR", MB_ICONERROR | MB_OK);
    return 0;
  }

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);  
    DispatchMessage(&msg); 
  }

  return msg.wParam;
}

LRESULT CALLBACK MyWindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  HDC hDC;
  PAINTSTRUCT ps;
  SYSTEMTIME st;
  INT Cx, Cy, x, y;
  INT Radius;
  DOUBLE angle_hour, angle_min, angle_sec;
  CHAR Buf[100];
  INT len;
  MINMAXINFO *minmax;
  static INT screenW = 800, screenH = 600;
  static HDC hMemDC = NULL, hTempDC = NULL;
  static HBITMAP hBm = NULL;
  static HBITMAP hClockFace = NULL;
  static CHAR *WD[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

  switch (Msg)
  {
  case WM_GETMINMAXINFO:

    minmax = (MINMAXINFO *)lParam;
    minmax->ptMinTrackSize.x = 40;  
    minmax->ptMinTrackSize.y = 30; 
    minmax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER);
    return 0;
  case WM_CREATE:

    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC); 
    hTempDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    hClockFace = LoadImage(NULL, "es3_clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hClockFace == NULL)
    {
      MessageBox(hWnd, "Error loading clockface.bmp", "ERROR", MB_ICONERROR | MB_OK);
      return -1;
    }
    SelectObject(hTempDC, hClockFace);
    SetTimer(hWnd, 1, 50, NULL);
    return 0;
  case WM_SIZE:

    screenW = LOWORD(lParam);
    screenH = HIWORD(lParam);

    if (hBm != NULL)
      DeleteObject(hBm);
      
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, screenW, screenH);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 1, 0);
    return 0;
      
  case WM_KEYDOWN:
    if (wParam == VK_F11)
      FlipFullScreen(hWnd);
    return 0;

  case WM_TIMER:

    if (hMemDC == NULL || hBm == NULL)
        return 0;

    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(hMemDC, RGB(0, 0, 0));
    SetDCPenColor(hMemDC, RGB(255, 255, 255));
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, screenW, screenH);

    if (hClockFace != NULL)
    {
      BITMAP bm;

      GetObject(hClockFace, sizeof(BITMAP), &bm); 
    
      x = (screenW - bm.bmWidth) / 2;
      y = (screenH - bm.bmHeight) / 2;
     
      BitBlt(hMemDC, x, y, bm.bmWidth, bm.bmHeight, hTempDC, 0, 0, SRCCOPY);
      
      Cx = screenW / 2;
      Cy = screenH / 2;
      Radius = (bm.bmWidth < bm.bmHeight ? bm.bmWidth : bm.bmHeight) / 2 - 10;
    }
    else
    {
      Cx = screenW / 2;
      Cy = screenH / 2;
      Radius = (screenW < screenH ? screenW : screenH) / 2 - 50;
        
      Ellipse(hMemDC, Cx - Radius, Cy - Radius, Cx + Radius, Cy + Radius);
    }
    
    GetLocalTime(&st);
    
    angle_sec = st.wSecond * 2 * PI / 60;
    
    angle_min = (st.wMinute + st.wSecond / 60.0) * 2 * PI / 60;
    
    angle_hour = ((st.wHour % 12) + st.wMinute / 60.0) * 2 * PI / 12;
   
    DrawHand(hMemDC, Cx, Cy, (INT)(Radius * 0.5), 8, angle_hour, 0, 0, 0);
  
    DrawHand(hMemDC, Cx, Cy, (INT)(Radius * 0.7), 5, angle_min, 0, 0, 0);
 
    DrawHand(hMemDC, Cx, Cy, (INT)(Radius * 0.85), 2, angle_sec, 0, 0, 0);
 
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(hMemDC, RGB(0, 0, 0));
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));
    Ellipse(hMemDC, Cx - 5, Cy - 5, Cx + 5, Cy + 5);

    SetBkMode(hMemDC, TRANSPARENT);  
    SetTextColor(hMemDC, RGB(0, 0, 0));
 
    len = wsprintf(Buf, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
    TextOut(hMemDC, screenW - 120, screenH - 50, Buf, len);

    len = wsprintf(Buf, "%02d.%02d.%04d (%s)", 
                   st.wDay, st.wMonth, st.wYear, WD[st.wDayOfWeek]);
    TextOut(hMemDC, screenW - 250, screenH - 30, Buf, len);

    InvalidateRect(hWnd, NULL, FALSE);

    return 0;

  case WM_PAINT:

    hDC = BeginPaint(hWnd, &ps);
    if (hMemDC != NULL && hBm != NULL)
    {
      BitBlt(hDC, 0, 0, screenW, screenH, hMemDC, 0, 0, SRCCOPY);
    }
      
    EndPaint(hWnd, &ps);
    return 0;

  case WM_ERASEBKGND:
    return 1;
      
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure close window?", "Exit", 
                  MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;  
    break; 
      
  case WM_DESTROY:

    if (hClockFace != NULL)
      DeleteObject(hClockFace); 
    if (hBm != NULL)
      DeleteObject(hBm);
    if (hMemDC != NULL)
      DeleteDC(hMemDC);
    if (hTempDC != NULL)
      DeleteDC(hTempDC);
    
    KillTimer(hWnd, 1); 
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
