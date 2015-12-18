#pragma once
#include <tchar.h>
#include <windows.h>

static TCHAR ButtonClass[] = _T("Button");

/* Caption's buttons's handle */
extern HWND hWndClose;
extern HWND hWndMin;
extern HWND hWndInfo;
extern HWND hWndAlgo1;
extern HWND hWndAlgo2;

bool regisButton(void);
bool unregisButton(void);