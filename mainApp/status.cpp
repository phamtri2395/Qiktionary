#include "status.h"
#include "vars.h"

std::wstring statusMSG;
bool avlProgress = false;
bool hashProgress = false;



void reDraw(HWND hWnd) {
	RECT client;
	GetClientRect(hWnd, &client);

	InvalidateRect(hWnd, &client, TRUE);
}
void paintStatus(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc;

	/* Paint Status */
	hdc = BeginPaint(hWnd, &ps);
	SetTextColor(hdc, 0xFFFFFF);
	SetBkMode(hdc, TRANSPARENT);

	/* Select Font */
	HFONT hFont;
	hFont = (HFONT)CreateFont(
		FONT_SMALL_HEIGHT, FONT_SMALL_WIDTH,
		0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_NATURAL_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		FONT_BOLD);
	if (hFont) {
		SelectObject(hdc, hFont);
		DeleteObject(hFont);
	}

	RECT client;
	GetClientRect(hWnd, &client);

	statusMSG.clear();
	statusMSG.append(L"AVL: ");
	statusMSG.append(avlProgress ? L"READY   " : L"Loading...     ");
	statusMSG.append(L"HASH: ");
	statusMSG.append(hashProgress ? L"READY" : L"Loading...");

	HDC compatible = CreateCompatibleDC(hdc);
	
	TextOut(hdc, STATUS_MARGIN_LEFT, (STATUS_HEIGHT - FONT_SMALL_HEIGHT) / 2, statusMSG.c_str(), statusMSG.length());

	BitBlt(hdc, 0, 0, client.right, client.bottom, compatible, 0, 0, SRCCOPY);

	DeleteDC(compatible);

	EndPaint(hWnd, &ps);
}





LRESULT CALLBACK StatusProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_STATUS_LOADING_AVL:
		break;

	case WM_STATUS_READY_AVL:
		avlProgress = true;
		reDraw(hWnd);
		break;

	case WM_STATUS_LOADING_HASH:
		break;

	case WM_STATUS_READY_HASH:
		hashProgress = true;
		reDraw(hWnd);
		break;

	case WM_PAINT:
		paintStatus(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}





/* Register & Unregister */
bool regisStatus(void) {
	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = StatusProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = StatusClass;
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(STATUS_BACKGROUND);

	if (!RegisterClass(&wc)) {
		MessageBox(NULL,
			_T("Failed to register Status Bar"),
			_T("Failed"),
			MB_ICONERROR);

		return false;
	}

	return true;
}
bool unregisStatus(void) {
	if (!UnregisterClass(StatusClass, NULL)) {
		MessageBox(NULL,
			_T("Failed to unregister Status Bar"),
			_T("Failed"),
			MB_ICONERROR);

		return false;
	}

	return true;
}