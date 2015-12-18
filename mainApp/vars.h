#pragma once

/* Global variables */
extern const char* file1;
extern const char* file2;

extern char algorithm;
extern char* outText;
extern int outtextLength;

/* IDs */
#define IDC_CAPTION 101
#define IDC_SOURCETEXT 102
#define IDC_DESTFIELD 103
#define IDC_INFO 104
#define IDC_MIN 105
#define IDC_CLOSE 106
#define IDC_ALGO1 107
#define IDC_ALGO2 108
#define IDC_STATUS 109

/* Window's properties */
#define WINDOW_BACKGROUND 0xFFFFFF
#define WINDOW_WIDTH 360
#define WINDOW_HEIGHT 640
#define WINDOW_TRANSPARENT_RATIO 60
#define TITLE_NAME "Qiktionary"
#define TITLE_MARGIN 15

/* Window's parameter */
extern HINSTANCE rootInst;
extern HWND roothWnd;

/* Caption */
#define CAPTION_HEIGHT 40
#define CAPTION_BACKGROUND RGB(32, 62, 121)

/* Source Field */
#define SOURCE_HEIGHT 270
#define TEXT_LENGTH 2000
#define TEXT_MARGIN 5

/* Source Field button*/
#define SOURCE_BUTTON_HEIGHT 20

/* Dest Field */
#define DEST_MARGIN_TOP 10
#define DEST_MARGIN_BOTTOM 5
#define DEST_MARGIN_LEFT 10
#define DEST_BACKGROUND RGB(0, 122, 204)
extern int DEST_FONT_HEIGHT;
extern int DEST_FONT_WIDTH;

/* Caption Button */
#define BUTTON_COLOR_NORMAL_CAP RGB(32, 62, 121)
#define BUTTON_COLOR_GLOW_CAP RGB(51, 102, 204)
#define BUTTON_WIDTH_CAP 32
#define BUTTON_HEIGHT_CAP 20

/* Source Button */
#define BUTTON_COLOR_NORMAL_SRC 0xFFFFFF
#define BUTTON_COLOR_GLOW_SRC RGB(32, 62, 121)
#define BUTTON_WIDTH_SRC 32
#define BUTTON_HEIGHT_SRC 20

/* Status */
#define STATUS_HEIGHT 20
#define STATUS_BACKGROUND RGB(32, 62, 121)
#define STATUS_MARGIN_LEFT 5
#define STATUS_LOADING L"Loading..."
#define STATUS_READY L"READY"

//
#define DEST_HEIGHT (WINDOW_HEIGHT - CAPTION_HEIGHT - SOURCE_HEIGHT - SOURCE_BUTTON_HEIGHT - STATUS_HEIGHT - DEST_MARGIN_TOP - DEST_MARGIN_BOTTOM)
#define DEST_WIDTH (WINDOW_WIDTH - DEST_MARGIN_LEFT * 2)
#define DEST_HEIGHT_CURRENT (FONT_LARGE_HEIGHT * floor(FONT_LARGE_WIDTH * outtextLength / DEST_WIDTH + 1) + DEST_MARGIN_BOTTOM)

/* FONTS */
#define FONT_LIGHT L"Clear Sans Light"
#define FONT_REGULAR L"Clear Sans Regular"
#define FONT_BOLD L"Clear Sans Bold"
#define FONT_LARGE_WIDTH 11
#define FONT_LARGE_HEIGHT 24
#define FONT_MEDIUM_WIDTH 8
#define FONT_MEDIUM_HEIGHT 20
#define FONT_SMALL_WIDTH 6
#define FONT_SMALL_HEIGHT 15
#define FONT_TINY_WIDTH 4
#define FONT_TINY_HEIGHT 12



/* Self-defined Messages */
#define WM_FIND_DENOTATION (WM_USER + 0x0001)
#define WM_CHANGE_ALGORITHM (WM_USER + 0x0002)
#define WM_STATUS_LOADING_AVL (WM_USER + 0x0003)
#define WM_STATUS_READY_AVL (WM_USER + 0x0004)
#define WM_STATUS_LOADING_HASH (WM_USER + 0x0005)
#define WM_STATUS_READY_HASH (WM_USER + 0x0006)