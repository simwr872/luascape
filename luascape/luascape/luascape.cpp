// luascape.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "StringHelp.h"
#include <commdlg.h>
#include <iostream>
#include "luascape.h"
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


static const unsigned int WIDTH = 800;
static const unsigned int HEIGHT = 600;
HWND runescapeWindow;
HWND runescapeClient;
long runescapeStyle;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



string GetWorkingDirectory() {
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);
	string dir = w2s(path);
	dir.erase(dir.rfind('\\'));

	return dir;
}


OPENFILENAME ScriptFiles(HWND hWnd) {
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.hwndOwner = hWnd;
	ofn.lStructSize = sizeof(OPENFILENAME);
	//TODO: PREPEND '/script' DIRECTORY
	ofn.lpstrInitialDir = s2w(GetWorkingDirectory().c_str());
	ofn.lpstrFilter = L"Lua files (.lua)\0*.LUA";
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	return ofn;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	if (true) {
		AllocConsole();
		FILE *sin;
		freopen_s(&sin, "CONIN$", "r", stdin);
		FILE *sout;
		freopen_s(&sout, "CONOUT$", "w", stdout);
		FILE *serr;
		freopen_s(&serr, "CONOUT$", "w", stderr);
	}

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LUASCAPE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LUASCAPE));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LUASCAPE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LUASCAPE);
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
	runescapeWindow = FindWindow(NULL, L"RuneScape");
	runescapeClient = GetWindow(runescapeWindow, GW_CHILD);
	runescapeStyle = GetWindowLong(runescapeWindow, GWL_STYLE);
	SetWindowLong(runescapeWindow, GWL_STYLE, runescapeStyle & ~(WS_CAPTION | WS_SIZEBOX));
	SetWindowPos(runescapeWindow, NULL, NULL, NULL, WIDTH, HEIGHT, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	EnableWindow(runescapeClient, false);

	hInst = hInstance;
	RECT rect { 0, 0, WIDTH, HEIGHT };
	DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
	AdjustWindowRectEx(&rect, style, true, WS_EX_LAYERED);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	HWND hWnd = CreateWindowEx(WS_EX_LAYERED, szWindowClass, szTitle, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, runescapeWindow, NULL, hInstance, NULL);
	if (!hWnd) return FALSE;

	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
	SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG) brush);
	SetLayeredWindowAttributes(hWnd, RGB(255, 0, 255), 0, LWA_COLORKEY);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

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
		case WM_MOVE: {
			//TODO: MOVE RUNESCAPE WINDOW UPON MOVING OUR WINDOW
			break;
		}
		case WM_COMMAND: {
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId) {
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				case IDM_SCRIPT_LOAD: {

					wchar_t fn[MAX_PATH] = { 0 };
					OPENFILENAME ofn = ScriptFiles(hWnd);
					ofn.lpstrFile = fn;
					GetOpenFileName(&ofn);

					string filename = w2s(fn);
					cout << filename << endl;
					break;
				}
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
			//TODO: PROGRAM CLEANUP, DELETE POINTERS, RESET RUNESCAPE WINDOW
			EnableWindow(runescapeClient, true);
			SetWindowLong(runescapeWindow, GWL_STYLE, runescapeStyle);
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
			return (INT_PTR) TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR) TRUE;
			}
			break;
	}
	return (INT_PTR) FALSE;
}
