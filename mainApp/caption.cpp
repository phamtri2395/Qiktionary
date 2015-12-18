#include "caption.h"
#include "vars.h"

#include <windowsx.h>
#include "button.h"

/* Global variables */
bool f_trackcap = false;
int x_pos = 0;
int y_pos = 0;

/* Declare Caption's buttons's handle */
HWND hWndClose = NULL;
HWND hWndMin = NULL;
HWND hWndInfo = NULL;





void moveWindow(HWND hWnd, int x, int y) {
	RECT rect;
	GetWindowRect(hWnd, &rect);

	SetWindowPos(hWnd, 0, rect.left + (x - x_pos), rect.top + (y - y_pos), 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
}

void CreateButtonCap(HWND hWnd) {
	/* Create Caption's buttons */
	hWndClose = CreateWindowEx( // Close button
		NULL,
		ButtonClass,
		NULL,
		WS_VISIBLE | WS_CHILD,
		WINDOW_WIDTH - BUTTON_WIDTH_CAP, 0,
		BUTTON_WIDTH_CAP, BUTTON_HEIGHT_CAP,
		hWnd,
		(HMENU)IDC_CLOSE,
		rootInst,
		NULL
		);
	if (!hWndClose) { // handle errors
		MessageBox(NULL,
			_T("Failed to create Close button"),
			_T("Failed"),
			MB_ICONERROR);
	}

	hWndMin = CreateWindowEx( // Min button
		NULL,
		ButtonClass,
		NULL,
		WS_VISIBLE | WS_CHILD,
		WINDOW_WIDTH - BUTTON_WIDTH_CAP * 2, 0,
		BUTTON_WIDTH_CAP, BUTTON_HEIGHT_CAP,
		hWnd,
		(HMENU)IDC_MIN,
		rootInst,
		NULL
		);
	if (!hWndMin) { // handle errors
		MessageBox(NULL,
			_T("Failed to create Min button"),
			_T("Failed"),
			MB_ICONERROR);
	}

	hWndInfo = CreateWindowEx( // Info button
		NULL,
		ButtonClass,
		NULL,
		WS_VISIBLE | WS_CHILD,
		WINDOW_WIDTH - BUTTON_WIDTH_CAP * 3, 0,
		BUTTON_WIDTH_CAP, BUTTON_HEIGHT_CAP,
		hWnd,
		(HMENU)IDC_INFO,
		rootInst,
		NULL
		);
	if (!hWndInfo) { // handle errors
		MessageBox(NULL,
			_T("Failed to create Info button"),
			_T("Failed"),
			MB_ICONERROR);
	}
}

void paintCaption(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc;

	/* Paint Title */
	hdc = BeginPaint(hWnd, &ps);
	SetTextColor(hdc, 0xFFFFFF);
	SetBkMode(hdc, TRANSPARENT);

	/* Select Font */
	/* Select Font */
	HFONT hFont;
	hFont = (HFONT)CreateFont(
		FONT_MEDIUM_HEIGHT, FONT_MEDIUM_WIDTH,
		0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_NATURAL_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		FONT_REGULAR);
	if (hFont) {
		SelectObject(hdc, hFont);
		DeleteObject(hFont);
	}


	TextOut(hdc, TITLE_MARGIN, (CAPTION_HEIGHT - FONT_MEDIUM_HEIGHT) / 2, _T(TITLE_NAME), 10);

	EndPaint(hWnd, &ps);
}





LRESULT CALLBACK CaptionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		CreateButtonCap(hWnd);
		break;

	case WM_PAINT:
		paintCaption(hWnd);
		break;

	case WM_MOUSEMOVE:
		if (wParam == 0x0001) // If Left Mouse button is down
		{
			int x_npos = GET_X_LPARAM(lParam);
			int y_npos = GET_Y_LPARAM(lParam);

			moveWindow(roothWnd, x_npos, y_npos);
		}
		break;
	case WM_LBUTTONDOWN: // Set focus & Track mouse
		SetFocus(hWnd);
		if (!f_trackcap) {
			f_trackcap = true;
			x_pos = GET_X_LPARAM(lParam);
			y_pos = GET_Y_LPARAM(lParam);

			SetLayeredWindowAttributes(roothWnd, 0, (255 * WINDOW_TRANSPARENT_RATIO) / 100, LWA_ALPHA); // Make window transparent
		}
		break;
	case WM_LBUTTONUP:
		SetFocus(roothWnd);
		if (f_trackcap) {
			f_trackcap = false;

			SetLayeredWindowAttributes(roothWnd, 0, 255, LWA_ALPHA); // Make window %0 transparent
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}





/* Register & Unregister */
bool regisCaption(void) {
	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = CaptionProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = CaptionClass;
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(CAPTION_BACKGROUND);

	if (!RegisterClass(&wc)) {
		MessageBox(NULL,
			_T("Failed to register Caption Bar"),
			_T("Failed"),
			MB_ICONERROR);

		return false;
	}

	return true;
}
bool unregisCaption(void) {
	if (!UnregisterClass(CaptionClass, NULL)) {
		MessageBox(NULL,
			_T("Failed to unregister Caption Bar"),
			_T("Failed"),
			MB_ICONERROR);

		return false;
	}

	return true;
}