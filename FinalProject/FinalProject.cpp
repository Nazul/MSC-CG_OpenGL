// FinalProject.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FinalProject.h"
#include "World.h"

#define MAX_LOADSTRING 100
#define IDT_TIMER1 1 


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
CWorld* world;
HCURSOR hShovelCursor;
HWND sb;
BOOL bPressed = FALSE;
BOOL bRaised = FALSE;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR    lpCmdLine,
  _In_ int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_FINALPROJECT, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FINALPROJECT));

  MSG msg;

  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FINALPROJECT));
  //wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FINALPROJECT);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance; // Store instance handle in our global variable

  HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd) {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  // World initialization
  world = CWorld::CreateWorld(hWnd);

  // Set timer for window refresh while idle
  SetTimer(hWnd,      // handle to main window 
    IDT_TIMER1,       // timer identifier 
    300,              //  interval (miliseconds)
    (TIMERPROC)NULL); // no timer callback 

  return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // Parse the menu selections:
    switch (wmId) {
    case ID_SIZE_SMALL:
    {
      HMENU hMenu = GetMenu(hWnd);
      MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
      mii.fMask = MIIM_STATE;
      mii.fState = MFS_CHECKED;
      SetMenuItemInfo(hMenu, ID_SIZE_SMALL, FALSE, &mii);
      mii.fState = MFS_UNCHECKED;
      SetMenuItemInfo(hMenu, ID_SIZE_MEDIUM, FALSE, &mii);
      SetMenuItemInfo(hMenu, ID_SIZE_LARGE, FALSE, &mii);
      world->SetAreaSize(AreaSize::Small);
    }
    break;
    case ID_SIZE_MEDIUM:
    {
      HMENU hMenu = GetMenu(hWnd);
      MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
      mii.fMask = MIIM_STATE;
      mii.fState = MFS_CHECKED;
      SetMenuItemInfo(hMenu, ID_SIZE_MEDIUM, FALSE, &mii);
      mii.fState = MFS_UNCHECKED;
      SetMenuItemInfo(hMenu, ID_SIZE_SMALL, FALSE, &mii);
      SetMenuItemInfo(hMenu, ID_SIZE_LARGE, FALSE, &mii);
      world->SetAreaSize(AreaSize::Medium);
    }
    break;
    case ID_SIZE_LARGE:
    {
      HMENU hMenu = GetMenu(hWnd);
      MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
      mii.fMask = MIIM_STATE;
      mii.fState = MFS_CHECKED;
      SetMenuItemInfo(hMenu, ID_SIZE_LARGE, FALSE, &mii);
      mii.fState = MFS_UNCHECKED;
      SetMenuItemInfo(hMenu, ID_SIZE_SMALL, FALSE, &mii);
      SetMenuItemInfo(hMenu, ID_SIZE_MEDIUM, FALSE, &mii);
      world->SetAreaSize(AreaSize::Large);
    }
    break;
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  break;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    RECT rect;
    if (world != NULL) {
      GetClientRect(hWnd, &rect);
      world->Resize(rect.left + rect.right, rect.top + rect.bottom);
      world->Display();
    }
    SendMessage(sb, WM_SIZE, NULL, NULL);
    EndPaint(hWnd, &ps);
  }
  break;
  case WM_SIZING:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    RECT rect;
    if (world != NULL) {
      GetClientRect(hWnd, &rect);
      world->Resize(rect.left + rect.right, rect.top + rect.bottom);
      world->Display();
    }
    EndPaint(hWnd, &ps);
  }
  break;
  case WM_TIMER:
    switch (wParam) {
    case IDT_TIMER1:
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      RECT rect;
      if (world != NULL) {
        GetClientRect(hWnd, &rect);
        world->Resize(rect.left + rect.right, rect.top + rect.bottom);
        world->Display();
      }
      EndPaint(hWnd, &ps);
    }
    break;
    // Prevents flick while resizing
  case WM_ERASEBKGND:
    break;
  case WM_MOUSEMOVE:
  {
    POINT curpos;
    GetCursorPos(&curpos);
    ScreenToClient(hWnd, &curpos);
    world->MouseMove(curpos.x, curpos.y);
    if (bPressed)
      world->Press();
    if (bRaised)
      world->Raise();
  }
  break;
  case WM_LBUTTONDOWN:
    world->Press();
    bPressed = TRUE;
    break;
  case WM_LBUTTONUP:
    bPressed = FALSE;
    break;
  case WM_RBUTTONDOWN:
    world->Raise();
    bRaised = TRUE;
    break;
  case WM_RBUTTONUP:
    bRaised = FALSE;
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}

// EOF
