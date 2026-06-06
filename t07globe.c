#include <windows.h>
#include "globe.h"

#define WND_CLASS_NAME "My CGSG SummerPractice 2026 Globe"

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

    SetWindowPos(hWnd, HWND_TOP,
        rc.left, rc.top,
        rc.right - rc.left,
        rc.bottom - rc.top,
        SWP_NOZORDER);

    IsFullScreen = TRUE;
}
  else
  {
    SetWindowPos(hWnd, HWND_TOP,
        SaveRect.left, SaveRect.top,
        SaveRect.right - SaveRect.left,
        SaveRect.bottom - SaveRect.top,
        SWP_NOZORDER);

    IsFullScreen = FALSE;
  }
}

LRESULT CALLBACK MyWindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  HDC hDC;
  PAINTSTRUCT ps;
  static HDC hMemDC = NULL;
  static HBITMAP hBm = NULL;
  static INT W = 800, H = 600;
  MINMAXINFO *minmax;
  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam; 
    minmax->ptMinTrackSize.y += 100; 
    minmax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    GLB_Init(0.30);

    SetTimer(hWnd, 1, 47, NULL);
    return 0;
  
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);

    GLB_Resize(W, H);

    if (hBm != NULL)
        DeleteObject(hBm);

    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
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
    SetDCPenColor(hMemDC, RGB(255, 255, 255));
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, W, H);

    GLB_Draw(hMemDC);

    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    if (hMemDC != NULL && hBm != NULL)
        BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);

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
    if (hBm != NULL)
        DeleteObject(hBm);
    if (hMemDC != NULL)
        DeleteDC(hMemDC);

    KillTimer(hWnd, 1);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
CHAR *CmdLine, INT CmdShow)
{
  HWND hWnd;
  WNDCLASS wc;
  MSG msg;
  
  wc.style = CS_HREDRAW | CS_VREDRAW;
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
  
  hWnd = CreateWindow(WND_CLASS_NAME, "T07GLOBE - Sphere",
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
                    100, 100, 800, 600, NULL, NULL, hInstance, NULL);
  
  if (!hWnd)
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

