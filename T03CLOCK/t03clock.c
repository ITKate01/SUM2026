#include <windows.h>
#include <math.h>
#define ARROW_COUNT 30 //кол-во стрелок
#define WND_CLASS_NAME "My CGSG SummerPractice'2026 Window class name"

/* Forward declaration */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );
VOID DrawArrow(HDC hDC, POINT *Ms, INT Xc, INT Yc, INT L, INT W)
{
  INT 
    i,
    dx = Ms->x-Xc,
    dy = Ms->y-Yc;
  DOUBLE 
    len = sqrt(dx*dx+dy*dy),
    angle;//угол поворота
  INT 
    x, y;
  INT
    x_a,
    y_a;
  POINT 
    pts[3] = {{0, 0}, {30, 30}, {-10, 22}},
    pts_res[sizeof(pts)/sizeof(pts[0])];

  INT N = sizeof(pts) / sizeof(pts[0]);

  if (len > L)
  {
    x = Xc + (INT)(dx * L/ len);
    y = Yc + (INT)(dy * L/ len);
  }
  else
  {
    x = Ms->x;
    y = Ms->y;
  }

  angle = atan2(dy, dx);

  for(i=0; i< N; i++)
  {
    x_a = (INT)(pts[i].x * cos(angle) - pts[i].y * sin(angle));
    y_a = (INT)(pts[i].x * sin(angle) + pts[i].y * cos(angle));
    pts_res[i].x = Xc + x_a;
    pts_res[i].y = Yc - y_a;
  }

  MoveToEx(hDC, Xc, Yc, NULL);
  LineTo(hDC, x, y);
  Polygon(hDC , pts_res, N);
}
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  HWND hWnd;
  WNDCLASS wc;
  MSG msg;

  /* Window class register */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
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

  /* Create window */
  hWnd = CreateWindow(WND_CLASS_NAME, "SummerPractice2026", WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    100, 300, 800, 500, NULL, NULL, hInstance, NULL);
  /*
  CreateWindow("BUTTON", "Press me...", WS_CHILD | WS_VISIBLE,
    10, 10, 100, 30, hWnd, (HMENU)123, hInstance, NULL);
  CreateWindow("BUTTON", "Expand", WS_CHILD | WS_VISIBLE,
    10, 47, 100, 30, hWnd, (HMENU)124, hInstance, NULL);
  CreateWindow("BUTTON", "Select me", BS_CHECKBOX | WS_CHILD | WS_VISIBLE | WS_BORDER,
    10, 84, 100, 30, hWnd, (HMENU)125, hInstance, NULL);
                                                                    */
  /* Main program loop */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
} /* End of 'WinMain' function */

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  INT i;
 /* INT x, y;    */
  HDC hDC;
  PAINTSTRUCT ps; 
  POINT pt;
  static INT W = 800, H=600;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 100, NULL);
    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
   /* SendMessage(hWnd, WM_TIMER, 30, 0);   */
    return 0;
  case WM_TIMER:
    hDC = GetDC(hWnd);

    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(hDC, RGB(0, 255, 0));
    SetDCBrushColor(hDC, RGB(255, 255, 255));
    Rectangle(hDC, 0, 0, W, H);  

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    srand(30);

    for(i = 0; i < ARROW_COUNT; i++)
    {
      SetDCPenColor(hDC, RGB(rand() & 255, rand() & 255, rand() & 255));
      SetDCBrushColor(hDC, RGB(rand() & 255, rand() & 255, rand() & 255));
      DrawArrow(hDC, &pt, rand() % W, rand() % H, 47, 18);
    }
    ReleaseDC(hWnd, hDC);
   /* InvalidateRect(hWnd, NULL, TRUE);*/
    return 0;
  case WM_ERASEBKGND:
    return 1;
  /*
  case WM_COMMAND:
    if (LOWORD(wParam) == 123)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    else if (LOWORD(wParam) == 124)
    {
      DWORD style = GetWindowLong(hWnd, GWL_STYLE);

      if (style & WS_MAXIMIZE)
        ShowWindow(hWnd, SW_NORMAL);
      else
        ShowWindow(hWnd, SW_MAXIMIZE);
       DestroyWindow((HWND)lParam); 
    }
    else if (LOWORD(wParam) == 125)
    {
      DWORD state = SendMessage((HWND)lParam, BM_GETSTATE, 0, 0);

      is_select = !!(state & BST_CHECKED);

      if (is_select)
        SendMessage((HWND)lParam, BM_SETCHECK, BST_UNCHECKED, 0);
      else
        SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED, 0);
      InvalidateRect(hWnd, NULL, FALSE);
    }
    return 0;  */
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
   /*
    SelectObject(hDC, GetStockObject(DC_BRUSH)); 
    SetDCBrushColor(hDC, RGB(30, 155, 155));
    Rectangle(hDC, 100, 100, 300, 200);     */
    
    /* GDI */
    /*
    if (is_select)
      SelectObject(hDC, GetStockObject(NULL_BRUSH));
    else
      SelectObject(hDC, GetStockObject(WHITE_BRUSH));
    SelectObject(hDC, GetStockObject(DC_PEN));
    SetDCPenColor(hDC, RGB(255, 0, 0));  */

   /* Ellipse(hDC, 0, 0, W, H);  */
    /*
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    SetDCPenColor(hDC, RGB(0, 255, 0));
    Ellipse(hDC, pt.x - 5, pt.y - 5, pt.x + 5, pt.y + 5);  */

    /*
    GetLocalTime(&st);
    TextOut(hDC, 130, 30, Buf, wsprintf(Buf, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond));      */    

    EndPaint(hWnd, &ps);
    return 0;
 /* case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;   */
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure close window?", "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
