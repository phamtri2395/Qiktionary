#include "dest.h"
#include "vars.h"

#include <windowsx.h>



void printText(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc;

	/* Paint result */
	hdc = BeginPaint(hWnd, &ps);
	SetTextColor(hdc, 0xffffff);
	SetBkMode(hdc, TRANSPARENT);

	/* Select Font */
	HFONT hFont;
	hFont = (HFONT)CreateFont(
		DEST_FONT_HEIGHT, DEST_FONT_WIDTH,
		0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_NATURAL_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		FONT_LIGHT);
	if (hFont) {
		SelectObject(hdc, hFont);
		DeleteObject(hFont);
	}

	wchar_t textResult[TEXT_LENGTH];
	if (outText) mbstowcs(textResult, outText, TEXT_LENGTH);

	RECT client;
	GetClientRect(hWnd, &client);
	client.top += DEST_MARGIN_BOTTOM / 2;
	if (outText) DrawText(hdc, textResult, outtextLength, &client, DT_CENTER | DT_WORDBREAK);

	EndPaint(hWnd, &ps);
}





LRESULT CALLBACK DestProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_PAINT:
		printText(hWnd);
		break;

	case WM_LBUTTONDOWN: // Set focus
		SetFocus(hWnd);
		break;
	case WM_LBUTTONUP:
		SetFocus(roothWnd);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}





/* Register & Unregister */
bool regisDest(void) {
	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DestProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = DestClass;
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(DEST_BACKGROUND);

	if (!RegisterClass(&wc)) {
		MessageBox(NULL,
			_T("Failed to register Dest Field"),
			_T("Failed"),
			MB_ICONERROR);

		return false;
	}

	return true;
}
bool unregisDest(void) {
	if (!UnregisterClass(DestClass, NULL)) {
		MessageBox(NULL,
			_T("Failed to unregister Dest Field"),
			_T("Failed"),
			MB_ICONERROR);

		return false;
	}

	return true;
}