#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <string.h>
#include <thread>

#include "vars.h"
#include "caption.h"
#include "dest.h"
#include "status.h"
#include "button.h"

#include "../hashCore/hashDict.h"
#include "../avlCore/avlDict.h"

/* Global Variables */
const char* file1 = "./data/tu_dien.dat";
const char* file2 = "./data/tu_dien.dat";

char algorithm = 2;
hashCore* hashDictionary = NULL;
avlCore* avlDictionary = NULL;
void(*p_loadAVL)(const char*) = NULL;
void(*p_loadHASH)(const char*) = NULL;

char* outText = NULL;
int outtextLength = 0;
int DEST_FONT_WIDTH = FONT_LARGE_WIDTH;
int DEST_FONT_HEIGHT = FONT_LARGE_HEIGHT;

static TCHAR WindowClass[] = _T("win32App"); // Windows class's name
static TCHAR Title[] = _T("Qiktionary"); //Window's title
HINSTANCE rootInst;
HWND roothWnd = NULL;

HWND hWndCaption = NULL;
HWND hWndSource = NULL;
WNDPROC oldTextBoxProc;
HWND hWndDest = NULL;
HWND hWndAlgo1 = NULL;
HWND hWndAlgo2 = NULL;
HWND hWndStatus = NULL;
HANDLE hWndAVLThread = NULL;
HANDLE hWndHASHThread = NULL;

DWORD AVLThreadId = 1;
DWORD HASHThreadId = 2;



/* Threading */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI AVLThread(LPVOID lpParameter) {
	SendMessage(hWndStatus, WM_STATUS_LOADING_HASH, NULL, NULL);

	p_loadAVL(file1);

	SendMessage(hWndStatus, WM_STATUS_READY_AVL, NULL, NULL);

	return 0;
};
DWORD WINAPI HASHThread(LPVOID lpParameter) {
	SendMessage(hWndStatus, WM_STATUS_LOADING_HASH, NULL, NULL);

	p_loadHASH(file2);

	SendMessage(hWndStatus, WM_STATUS_READY_HASH, NULL, NULL);

	return 0;
};



void CreateCaption(HWND);
void CreateSourceText(HWND);
void CreateDestField(HWND);
void CreateSourceButton(HWND);
void CreateStatus(HWND);





/* Core Functions */
void loadAVL(const char*);
void loadHASH(const char*);
std::string verify(char*);
std::string findDenotation(const char*);





/* WinMain */
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	/* WNDCLASSEX */
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(WINDOW_BACKGROUND));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WindowClass;
	wcex.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));

	int test = GetLastError();

	if (!RegisterClassEx(&wcex)) { // Register window class, if failed throw message and end program
		MessageBox(NULL,
			_T("Failed to register Window's class"),
			_T("Failed"),
			MB_ICONERROR);

		return 1;
	}

	

	/* Register Control's class */
	regisCaption();
	regisDest();
	regisButton();
	regisStatus();



	/* Create Window and handle errors */
	HWND hWnd = CreateWindow(
		WindowClass,
		Title,
		WS_POPUPWINDOW,
		GetSystemMetrics(SM_CXSCREEN) / 2 - WINDOW_WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - WINDOW_HEIGHT / 2,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	if (!hWnd) { // throw message if can't create window
		MessageBox(NULL,
			_T("Failed to Create Window"),
			_T("Failed"),
			MB_ICONERROR);

		return 1;
	}

	rootInst = hInstance; // Store window's instance
	roothWnd = hWnd; // Store window's HWND

	// Create Controls
	CreateCaption(hWnd);
	CreateSourceText(hWnd);
	CreateDestField(hWnd);
	CreateSourceButton(hWnd);
	CreateStatus(hWnd);



	ShowWindow(hWnd, nCmdShow); // show & update window
	UpdateWindow(hWnd);
	SetFocus(hWnd);
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED); // Set Window to EX_LAYERED style
	SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA); // Make Window 0% transparent



	/* Create new thread for loading data */
	p_loadAVL = &loadAVL;
	p_loadHASH = &loadHASH;
	hWndAVLThread = CreateThread(0, 0, AVLThread, NULL, 0, &AVLThreadId);
	hWndHASHThread = CreateThread(0, 0, HASHThread, NULL, 0, &HASHThreadId);



	/* Handle messages */
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}





/* Window's main Procedure */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_FIND_DENOTATION:
		strcpy(outText, findDenotation(outText).c_str());

		outtextLength = strlen(outText);



		// Dynamic font size
		if (DEST_HEIGHT_CURRENT < DEST_HEIGHT)
		{
			DEST_FONT_WIDTH = FONT_LARGE_WIDTH;
			DEST_FONT_HEIGHT = FONT_LARGE_HEIGHT;

			SetWindowPos(hWndDest, 0, 0, 0,  // Set dest height
				DEST_WIDTH,
				DEST_HEIGHT_CURRENT,
				SWP_NOMOVE | SWP_SHOWWINDOW);
		}
		else if (DEST_HEIGHT_CURRENT < DEST_HEIGHT * 2) {
			DEST_FONT_WIDTH = FONT_MEDIUM_WIDTH;
			DEST_FONT_HEIGHT = FONT_MEDIUM_HEIGHT;

			SetWindowPos(hWndDest, 0, 0, 0,  // Set dest height
				DEST_WIDTH,
				DEST_HEIGHT,
				SWP_NOMOVE | SWP_SHOWWINDOW);
		}
		else {
			DEST_FONT_WIDTH = FONT_SMALL_WIDTH;
			DEST_FONT_HEIGHT = FONT_SMALL_HEIGHT;

			SetWindowPos(hWndDest, 0, 0, 0,  // Set dest height
				DEST_WIDTH,
				DEST_HEIGHT,
				SWP_NOMOVE | SWP_SHOWWINDOW);
		}



		RECT client;
		GetClientRect(hWndDest, &client);
		InvalidateRect(hWndDest, &client, TRUE);
		break;

	case WM_CHANGE_ALGORITHM:
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}





LRESULT CALLBACK TextBoxContentProc(HWND, UINT, WPARAM, LPARAM);
void CreateSourceText(HWND hWnd) {
	/* Create Textbox */
	hWndSource = CreateWindowEx(
		NULL,
		_T("EDIT"),
		NULL,
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
		-1, CAPTION_HEIGHT,
		WINDOW_WIDTH, SOURCE_HEIGHT,
		hWnd,
		(HMENU)IDC_SOURCETEXT,
		rootInst,
		NULL
		);
	if (!hWndSource) { // handle errors
		MessageBox(NULL,
			_T("Failed to create Source field"),
			_T("Failed"),
			MB_ICONERROR);
	}

	/* Subclassing Textbox */
	oldTextBoxProc = (WNDPROC)SetWindowLongPtr(hWndSource, GWLP_WNDPROC, (LONG_PTR)TextBoxContentProc);

	/* Select Font */
	HFONT hFont = (HFONT)CreateFont(
		FONT_LARGE_HEIGHT, FONT_LARGE_WIDTH,
		0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_NATURAL_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		FONT_LIGHT);
	/* Set TextBox's font */
	SendMessage(hWndSource, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

	/* Set Margins */
	SendMessage(hWndSource, EM_SETMARGINS, EC_USEFONTINFO, 10);
}

/* TextBox's Procedure */
LRESULT CALLBACK TextBoxContentProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CHAR:
		switch (wParam) {
		case VK_RETURN:
			wchar_t text[TEXT_LENGTH];
			GetWindowText(hWnd, text, TEXT_LENGTH); // Get textbox's text

			if (!outText) outText = new char[TEXT_LENGTH];
			if (text[0] != _T('\0')) { wcstombs(outText, text, TEXT_LENGTH); } // Convert from WCHAR_T to CHAR*
			else {
				delete outText;
				break;
			}
			outtextLength = strlen(outText);

			SendMessage(roothWnd, WM_FIND_DENOTATION, NULL, NULL);

			break;

		default:
			return CallWindowProc(oldTextBoxProc, hWnd, message, wParam, lParam);
		}

		break;

	default:
		return CallWindowProc(oldTextBoxProc, hWnd, message, wParam, lParam);
	}
}

void CreateCaption(HWND hWnd) {
	hWndCaption = CreateWindowEx(
		NULL,
		CaptionClass,
		NULL,
		WS_VISIBLE | WS_CHILD,
		0, 0,
		WINDOW_WIDTH, CAPTION_HEIGHT,
		hWnd,
		(HMENU)IDC_CAPTION,
		rootInst,
		NULL
		);
	if (!hWndCaption) { // handle errors
		MessageBox(NULL,
			_T("Failed to create Create Caption"),
			_T("Failed"),
			MB_ICONERROR);
	}
}
void CreateDestField(HWND hWnd) {
	hWndDest = CreateWindowEx(
		NULL,
		DestClass,
		NULL,
		WS_VISIBLE | WS_CHILD,
		DEST_MARGIN_LEFT, CAPTION_HEIGHT + SOURCE_HEIGHT + SOURCE_BUTTON_HEIGHT + DEST_MARGIN_TOP,
		WINDOW_WIDTH - DEST_MARGIN_LEFT * 2, 0,
		hWnd,
		(HMENU)IDC_DESTFIELD,
		rootInst,
		NULL
		);
	if (!hWndCaption) { // handle errors
		MessageBox(NULL,
			_T("Failed to create Dest Field"),
			_T("Failed"),
			MB_ICONERROR);
	}
}
void CreateSourceButton(HWND hWnd) {
	/* Create Source's buttons */
	hWndAlgo1 = CreateWindowEx( // Algorithm 1 button
		NULL,
		ButtonClass,
		NULL,
		WS_VISIBLE | WS_CHILD,
		WINDOW_WIDTH - BUTTON_WIDTH_SRC, CAPTION_HEIGHT + SOURCE_HEIGHT,
		BUTTON_WIDTH_SRC, BUTTON_HEIGHT_SRC,
		hWnd,
		(HMENU)IDC_ALGO1,
		rootInst,
		NULL
		);
	if (!hWndAlgo1) { // handle errors
		MessageBox(NULL,
			_T("Failed to create Algorithm 1 button"),
			_T("Failed"),
			MB_ICONERROR);
	}

	hWndAlgo2 = CreateWindowEx( // Algorithm 2 button
		NULL,
		ButtonClass,
		NULL,
		WS_VISIBLE | WS_CHILD,
		WINDOW_WIDTH - BUTTON_WIDTH_SRC * 2, CAPTION_HEIGHT + SOURCE_HEIGHT,
		BUTTON_WIDTH_SRC, BUTTON_HEIGHT_SRC,
		hWnd,
		(HMENU)IDC_ALGO2,
		rootInst,
		NULL
		);
	if (!hWndAlgo2) { // handle errors
		MessageBox(NULL,
			_T("Failed to create Algorithm 2 button"),
			_T("Failed"),
			MB_ICONERROR);
	}
}
void CreateStatus(HWND hWnd) {
	hWndStatus = CreateWindowEx(
		NULL,
		StatusClass,
		NULL,
		WS_VISIBLE | WS_CHILD,
		0, WINDOW_HEIGHT - STATUS_HEIGHT,
		WINDOW_WIDTH, STATUS_HEIGHT,
		hWnd,
		(HMENU)IDC_STATUS,
		rootInst,
		NULL
		);
	if (!hWndStatus) { // handle errors
		MessageBox(NULL,
			_T("Failed to create Status Bar"),
			_T("Failed"),
			MB_ICONERROR);
	}
}




void loadAVL(const char* filename) {
	if (avlDictionary == NULL) {
		avlDictionary = new avlCore();

		/* Load data to AVL */
		if (!avlDictionary->load(filename)) MessageBox(NULL, _T("Load dictionary by AVL Tree failed!"), _T("Load dictionary failed"), MB_ICONERROR);
	}
}
void loadHASH(const char* filename) {
	if (hashDictionary == NULL) {
		hashDictionary = new hashCore();

		/* Load data to hash */
		if (!hashDictionary->load(filename)) MessageBox(NULL, _T("Load dictionary by Hash Table failed!"), _T("Load dictionary failed"), MB_ICONERROR);
	}
}

std::string verify(const char* str) {
	std::string temp = str;

	/* Remove trailing space */
	while (temp.find(' ') == 0) {
		temp.erase(0, 1);
	}
	while (temp.length() && (temp[temp.length() - 1] == ' ')) {
		temp.erase(temp.length() - 1, 1);
	}

	return temp;
}

std::string findDenotation(const char* str) {
	std::string temp = verify(str);
	std::string denotation;

	if (temp.length()) {
		while (temp.length() || (temp.find(' ') != std::string::npos)) {
			std::string word = temp.substr(0, temp.find(' '));

			std::string raw;
			if (word != "") {
				raw = ((algorithm == 1) ? avlDictionary->find(word.c_str()) : hashDictionary->find(word.c_str()));

				denotation.append((raw.find(',')) ? raw.substr(0, raw.find(',')) : raw);
				denotation.append(" ");
			}

			temp = temp.substr((temp.find(' ') != std::string::npos) ? temp.find(' ') + 1 : temp.length(), temp.length() - temp.find(' '));
		}
	}
	else return "";

	return denotation;
}