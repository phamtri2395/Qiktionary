#include "button.h"
#include "vars.h"
#include <string>

/* Global Variables */
bool f_trackbutt = false;
TRACKMOUSEEVENT tme;
extern bool avlProgress;
extern bool hashProgress;

/* Track mouse */
void trackMouse(HWND hWnd) {
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.hwndTrack = hWnd;
	tme.dwHoverTime = 1;
	TrackMouseEvent(&tme);
}

void paintButton(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;

	GetClientRect(hWnd, &rect);
	hdc = BeginPaint(hWnd, &ps);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);

	/* Paint different kind of button */
	if ((hWnd == hWndInfo) || (hWnd == hWndClose) || (hWnd == hWndMin)) {
		if (f_trackbutt) FillRect(hdc, &rect, CreateSolidBrush(BUTTON_COLOR_GLOW_CAP));
		else FillRect(hdc, &rect, CreateSolidBrush(BUTTON_COLOR_NORMAL_CAP));
	}
	else if (hWnd == hWndAlgo1) {
		if (algorithm == 1) FillRect(hdc, &rect, CreateSolidBrush(BUTTON_COLOR_GLOW_SRC));
		else FillRect(hdc, &rect, CreateSolidBrush(BUTTON_COLOR_NORMAL_SRC));
	}
	else if (hWnd == hWndAlgo2) {
		if (algorithm == 2) FillRect(hdc, &rect, CreateSolidBrush(BUTTON_COLOR_GLOW_SRC));
		else FillRect(hdc, &rect, CreateSolidBrush(BUTTON_COLOR_NORMAL_SRC));
	}



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
		FONT_REGULAR);
	if (hFont) {
		SelectObject(hdc, hFont);
		DeleteObject(hFont);
	}

	/* Paint Caption button */
	if (hWnd == hWndClose)
		DrawText(hdc, L"x", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	else if (hWnd == hWndMin)
		DrawText(hdc, L"_", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	else if (hWnd == hWndInfo)
		DrawText(hdc, L"?", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	/* Select Font */
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
	/* Paint Source button */
	if (hWnd == hWndAlgo1) {
		if (algorithm == 2) SetTextColor(hdc, RGB(32, 62, 121));
		DrawText(hdc, L"AVL", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	else if (hWnd == hWndAlgo2) {
		if (algorithm == 1) SetTextColor(hdc, RGB(32, 62, 121));
		DrawText(hdc, L"HASH", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}



	EndPaint(hWnd, &ps);

	ReleaseDC(hWnd, hdc);
}

void redrawButton(HWND hWnd) {
	RECT rect;
	GetClientRect(hWnd, &rect);
	InvalidateRect(hWnd, &rect, 1);
}

void DoButtonFunc(HWND hWnd) {
	if (hWnd == hWndClose)
		PostQuitMessage(0);
	else if (hWnd == hWndMin)
		ShowWindow(roothWnd, SW_SHOWMINIMIZED);
	else if (hWnd == hWndInfo) {
		std::wstring relPath;
		LPWSTR temp = new TCHAR[1000];
		GetModuleFileName(NULL, temp, 1000);

		relPath.append(L"file:///");
		relPath.append(temp);
		int lastpos = 0;
		while (relPath.find('\\') != std::string::npos) {
			lastpos = relPath.find('\\');
			relPath.replace(relPath.find('\\'), 1, L"/");
		}

		relPath.erase(lastpos, relPath.length() - lastpos);
		relPath.append(L"/data/help.html");

		ShellExecute(0, 0, relPath.c_str(), 0, 0 , SW_SHOW );
	}
	else if (hWnd == hWndAlgo1) {
		if (!avlProgress) {
			if (MessageBox(NULL, L"AVL dictionary is being loaded!\nSome words may not be available now yet.\n\nClick OK to continue switching dictionary!", L"Loading dictionary!", MB_ICONWARNING | MB_OKCANCEL) == IDOK) {
				MessageBox(NULL, L"Loading AVL dictionary may take up to 5min to finish causes its special property!", L"Information", MB_ICONINFORMATION | MB_OK);				
				doit1:
				algorithm = 1;

				redrawButton(hWndAlgo1);
				redrawButton(hWndAlgo2);

				SendMessage(roothWnd, WM_CHANGE_ALGORITHM, NULL, NULL); // send message to main app
			}
		} else goto doit1;
	}
	else if (hWnd == hWndAlgo2) {
		if (!hashProgress) {
			if(MessageBox(NULL, L"HASH dictionary is being loaded!\nSome words may not be available now yet.\n\nClick OK to continue switching dictionary!", L"Loading dictionary!", MB_ICONWARNING | MB_OKCANCEL) == IDOK) {
				doit2:
				algorithm = 2;

				redrawButton(hWndAlgo1);
				redrawButton(hWndAlgo2);
	
				SendMessage(roothWnd, WM_CHANGE_ALGORITHM, NULL, NULL); // send message to main app
			}
		} else goto doit2;
	}
}



LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_MOUSEMOVE:
		if (!f_trackbutt) trackMouse(hWnd);
		break;
	case WM_MOUSEHOVER:
		tme.dwFlags = TME_LEAVE;
		f_trackbutt = true;
		redrawButton(hWnd);
		break;
	case WM_MOUSELEAVE:
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		f_trackbutt = false;
		redrawButton(hWnd);
		break;

	case WM_LBUTTONDOWN: // Set focus & Do button's function
		SetFocus(hWnd);

		DoButtonFunc(hWnd);
		break;

	case WM_PAINT:
		paintButton(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}





/* Register & Unregister */
bool regisButton(void) {
	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ButtonProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = ButtonClass;
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(BUTTON_COLOR_NORMAL_CAP);

	if (!RegisterClass(&wc)) {
		MessageBox(NULL,
			_T("Failed to register Button"),
			_T("Failed"),
			MB_ICONERROR);

		return false;
	}

	return true;
}
bool unregisButton(void) {
	if (!UnregisterClass(ButtonClass, NULL)) {
		MessageBox(NULL,
			_T("Failed to unregister Button"),
			_T("Failed"),
			MB_ICONERROR);

		return false;
	}

	return true;
}