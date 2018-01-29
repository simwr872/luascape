// luascape.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "StringHelp.h"
#include <commdlg.h>
#include <iostream>
#include "luascape.h"
#include "lua.hpp"
#include <thread>
#include "Vec2.h"
#include <bitset>
#include <chrono>
#include "ScanCode.h"
using namespace std;

#define MAX_LOADSTRING 100

#define LUA_STOPPED 0
#define LUA_STOPPING 1
#define LUA_RUNNING 2

// Global Variables:
// TODO: recheck where used. hInst only used when
// spawning About box.
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND overlay;

static const int WIDTH = 800;
static const int HEIGHT = 600;
HWND runescapeWindow;
HWND runescapeClient;
long runescapeStyle;
int scriptStatus = LUA_STOPPED;
float mx, my, pmx, pmy;							// Previous and current mouse x & y

bool DEBUG = false;



// Forward declarations of functions included in this code module:
ATOM                RegisterOverlayClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Variables to handle resizing our overlay window accomodating
// for the titlebar and menu (aswell as hidden shadows taking
// up pixels). Also used when positioning the RuneScape window
// within ours - to make it line up.
RECT adjustedRect { 0, 0, WIDTH, HEIGHT };
DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX);



//
//   FUNCTION: ReadPixel()
//
//   PURPOSE:  Reads a pixel color from the RuneScape client.
//
void ReadPixel(int x, int y, int* color) {
	// Fetch the RuneScape client device context and
	// create another one compatible with it.
	HDC deviceContext = GetDC(runescapeClient);
	HDC deviceContextCopy = CreateCompatibleDC(deviceContext);

	// Define the bitmap. Height is inverted to make
	// the origin top-left instead of bitmaps default
	// bottom-left. 32 bitcount for all colors.
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = WIDTH;
	bitmap.bmiHeader.biHeight = -HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = WIDTH * 4 * HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;

	// Create and associate a bitpointer to the bits of
	// of our bitmap. We then select our bitmap and
	// copy all of RuneScape's buffer int ours.
	BYTE* bitPointer;
	HBITMAP hBitmap = CreateDIBSection(deviceContextCopy, &bitmap, DIB_RGB_COLORS, (void**) &bitPointer, NULL, NULL);
	SelectObject(deviceContextCopy, hBitmap);
	BitBlt(deviceContextCopy, 0, 0, WIDTH, HEIGHT, deviceContext, 0, 0, SRCCOPY);

	// Parse the colors, each pixel has 4 bytes of
	// information. (the 4th being alpha)
	int index		= y * WIDTH + x;
	int blue		= (int) bitPointer[index * 4 + 0];
	int green		= (int) bitPointer[index * 4 + 1];
	int red			= (int) bitPointer[index * 4 + 2];
	color[0] = red;
	color[1] = green;
	color[2] = blue;

	// Release RuneScape's device context aswell as
	// cleaning up what we created.
	ReleaseDC(runescapeClient, deviceContext);
	DeleteDC(deviceContextCopy);
	DeleteObject(hBitmap);

}


//
//   FUNCTION: GetWorkingDirectory()
//
//   PURPOSE:  Fetches the current working directory
//
string GetWorkingDirectory() {
	// Gets the working directory for our module.
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);

	// Converts the wide string into a string.
	// Directory returned actually describes the
	// exact file of our executable, so the part
	// after the last '\' is trimmed.
	string dir = w2s(path);
	dir.erase(dir.rfind('\\'));

	return dir;
}

//
//   FUNCTION: ScriptFiles(HWND hWnd)
//
//   PURPOSE:  Defines what files to be opened as scripts.
//
//   COMMENTS:
//
//        We only allow files of type .lua. The functions requires an HWND
//        which specifies the owner of the dialog.
//
OPENFILENAME ScriptFiles(HWND hWnd) {
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.hwndOwner = hWnd;
	ofn.lStructSize = sizeof(OPENFILENAME);

	//TODO: PREPEND '/script' DIRECTORY
	string pwd = GetWorkingDirectory();
	pwd.erase(pwd.rfind('\\'));
	pwd.erase(pwd.rfind('\\'));
	pwd.erase(pwd.rfind('\\'));
	pwd += "\\scripts";
	//TODO: PREPEND '/script' DIRECTORY

	ofn.lpstrInitialDir = s2w(pwd.c_str());
	ofn.lpstrFilter = L"Lua files (.lua)\0*.LUA";
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	return ofn;
}

// TODO: move to a separate class for handling
// with all kinds of mice?
float MouseDist(float x) {
	float ts = pow(x, 2);
	float tc = pow(x, 3);
	return 1 - (tc*ts + -5 * tc + 5 * ts);
}

void Move(float x, float y, WPARAM wParam = 0) {
	mx = x;
	my = y;
	PostMessage(runescapeClient, WM_MOUSEMOVE, wParam, MAKELPARAM(mx, my));
	InvalidateRect(overlay, NULL, NULL);
}

void SmoothMove(float x, float y) {
	float time = 500; // Change to separate function. Fitts law
	vec2 start(mx, my);
	vec2 goal(x, y);
	vec2 difference = goal - start;
	vec2 normal = goal.normal();
	vec2 current;
	float distance;
	for (float i = 0; i < time; i++) {
		distance = MouseDist(i / time);
		current = goal - distance*difference;
		Move(current.x, current.y);
		Sleep(1);
	}
	Move(current.x, current.y);
}

int LuaMove(lua_State *Lua) {
	// TODO: error checking
	float x = lua_tonumber(Lua, -2);
	float y = lua_tonumber(Lua, -1);
	lua_pop(Lua, 2);

	SmoothMove(x, y);
	return 0;
}



//
//   FUNCTION: PressKey(char c)
//
//   PURPOSE:  Sends a keypress to the RuneScape client.
//
bool PressKey(char c) {
	// Is the key actually valid?
	if (0 > c || c > 127 || ascii[c] == 0x00) return false;

	// LPARAM OF WM_KEYDOWN 
	// https://msdn.microsoft.com/en-us/library/ms646280(v=VS.85).aspx
	// |Transition|Previous |Context|Reserved|Extended|Scan    |Repeat count    |
	// |state     |key state|code   |        |key     |code    |                |
	// |0         |0        |0      |0000    |0       |00000000|0000000000000000|
	// Scan code is generated by hardware (keyboard) and is (usually) 
	// never used as a key identifier. Key scan code list;
	// https://msdn.microsoft.com/en-us/library/aa299374(v=vs.60).aspx

	// Convert the character to scan code and create both lparams
	// for key- down & up and char.
	bitset<8> scan(ascii[c]);
	bitset<32> lparam1("00000000" + scan.to_string() + "0000000000000001");
	bitset<32> lparam2("11000000" + scan.to_string() + "0000000000000001");

	// For keydown the transition state and context code must be set
	// to 0. We should only press keys one at a time so previos key
	// state can also be set to 0. We are not sending an extended
	// keypress (CTRL/ALT combo). Keydown always sends the capital
	// letter while wm_char sends it in the correct case.
	PostMessage(runescapeClient, WM_KEYDOWN, upper(c), lparam1.to_ullong());
	// For every keydown message there is also a char message.
	// LParam of this is exactly the same as for keydown.
	PostMessage(runescapeClient, WM_CHAR, c, lparam1.to_ullong());
	Sleep(rand() % (130 - 70 + 1) + 70);
	// For key up the transition state, previous key state and
	// repeat count must be set to 1. Context code must be 0.
	PostMessage(runescapeClient, WM_KEYUP, upper(c), lparam2.to_ullong());

	return true;
}

//
//   FUNCTION: LuaPress(lua_State *Lua)
//
//   PURPOSE:  Allows the script to press keys in RuneScape client.
//
int LuaPress(lua_State *Lua) {
	// TODO: TYPE CHECK
	string s = lua_tostring(Lua, -1);
	lua_pop(Lua, 1);
	
	char c = s.at(0);
	if (!PressKey(c)) luaL_error(Lua, ("Character '"+to_string(c)+"' not yet implemented!").c_str());

	return 0;
}


//
//   FUNCTION: LuaColor(lua_State *Lua)
//
//   PURPOSE:  Allows the script to read colors.
//
int LuaColor(lua_State *Lua) {
	// TODO: error checking AND CLEANUP
	float x = lua_tonumber(Lua, -2);
	float y = lua_tonumber(Lua, -1);
	lua_pop(Lua, 2);

	int color[3];
	ReadPixel(x, y, color);

	lua_newtable(Lua);
	for (int i = 0; i < 3; i++) {
		lua_pushnumber(Lua, i+1);		// key
		lua_pushnumber(Lua, color[i]);	// value
		lua_settable(Lua, -3);
	}

	return 1;
}

//
//   FUNCTION: LuaClick(lua_State *Lua)
//
//   PURPOSE:  Allows the script to click the RuneScape client.
//
int LuaClick(lua_State *Lua) {
	// Send a click message to the RuneScape client at
	// our current mouse coordinates with a delay before
	// we release the mouse. WParam must be set to
	// MK_LBUTTON (0x1) which marks that the left button
	// is being held
	PostMessage(runescapeClient, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(mx, my));
	// TODO: lognormal distribution AND record how long clicks actually are.
	Sleep(rand() % (120 - 60 + 1) + 60);
	// WParam must be 0x0 which marks that no other key
	// is being pressed
	PostMessage(runescapeClient, WM_LBUTTONUP, 0, MAKELPARAM(mx, my));

	// Lua functions return how many values a function
	// should return in the script. We return 0.
	return 0;
}

static const luaL_Reg LuaLibrary[] = {
	{ "click", LuaClick },
	{ "move", LuaMove },
	{ "press", LuaPress },
	{ "color", LuaColor },
	{ NULL, NULL }
};

//
//   FUNCTION: LuaHook(lua_State *Lua, lua_Debug *arg)
//
//   PURPOSE:  Checks if we should be exiting and throws error
//			   if that is the case.
//
void LuaHook(lua_State *Lua, lua_Debug *arg) {
	if (scriptStatus == LUA_STOPPING) luaL_error(Lua, "User terminated script.");
}

//
//   FUNCTION: RunLuaScript(string script)
//
//   PURPOSE:  Executes the users script.
//
//   COMMENTS:
//
//        This function should only be called from within a new thread.
//        It creates and executes code inside a lua state, which
//        periodically checks if we should be exiting the thread.
//
void RunLuaScript(string script) {
	// Disables the user from actually interacting with
	// the RuneScape client. Otherwise we might send two
	// different mouse locations at once(!).
	EnableWindow(runescapeClient, false);
	// Declare the script state as running
	scriptStatus = LUA_RUNNING;

	// Create a new lua state and load all default libraries.
	// Some libraries have been disabled such as parts of the
	// OS library, the entire IO and DEBUG library.
	// We also set our hook to check if we should be exiting
	// the thread. This occurs every 10k lines executed.
	// (10k might be a subject to change)
	lua_State* Lua = luaL_newstate();
	luaL_openlibs(Lua);
	lua_sethook(Lua, &LuaHook, LUA_MASKLINE, 0);
	// Load our own functions defined and make it available
	// to the script as 'client'.
	luaL_newlib(Lua, LuaLibrary);
	lua_setglobal(Lua, "client");

	// Loads the users file and executes it. If we are done
	// or an error occured - close the lua state and reset
	// script status. The thread will then exit on its own.
	int result = luaL_dofile(Lua, script.c_str());
	if (result != LUA_OK) {
		string s = lua_tostring(Lua, -1);
		MessageBox(NULL, s2w(s.c_str()), L"Lua status", MB_OK);
	}
	lua_close(Lua);
	scriptStatus = LUA_STOPPED;

	// Reenables interaction with the client.
	EnableWindow(runescapeClient, true);
}



int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
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
	RegisterOverlayClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LUASCAPE));

	MSG msg;
	int tt = 0;
	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);

			if (DEBUG) {
				if (msg.message == WM_KEYDOWN) {
					int c = msg.wParam;
					std::bitset<32> x(msg.lParam);
					cout << c << " - ";
					string s = "";
					for (int i = 23; i >= 16; i--) {
						s += to_string(x[i]);
					}
					std::bitset<80> a(s);
					cout << hex << a.to_ulong() << ", " << ascii[c] << dec;
					cout << endl;
				}
				/*cout << "-----" << endl;
				int time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				cout << time << endl;
				cout << hex << msg.message << endl;
				cout << hex << msg.wParam << dec << endl;

				//cout << hex << msg.lParam << dec << endl;
				std::bitset<32> x(msg.lParam);
				cout << x[31] << " " << x[30] << " " << x[29] << " ";
				for (int i = 28; i >= 25; i--) {
					cout << x[i];
				}
				cout << " " << x[24] << " ";
				for (int i = 23; i >= 16; i--) {
					cout << x[i];
				}
				cout << " ";
				for (int i = 15; i >= 0; i--) {
					cout << x[i];
				}
				cout << endl << "-----" << endl;*/
			}

			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: RegisterOverlayClass()
//
//  PURPOSE: Registers the window class.
//
ATOM RegisterOverlayClass(HINSTANCE hInstance) {
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
//   PURPOSE: Saves instance handle & RuneScape windows. Also creates main window.
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window. The RuneScape window and
//		  client are also saved in global variables.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	// Find the associated RuneScape window, used to find
	// the client area aswell as handling how the original
	// window actually looks like.
	runescapeWindow = FindWindow(NULL, L"RuneScape");

	// The runescape window only has one child (the client),
	// this is the HWND we send inputs to.
	runescapeClient = GetWindow(runescapeWindow, GW_CHILD);

	// Fetch the style used to reset upon application exit.
	runescapeStyle = GetWindowLong(runescapeWindow, GWL_STYLE);

	// Disable border (caption) and titlebar to make our
	// window only display the actual game.
	SetWindowLong(runescapeWindow, GWL_STYLE, runescapeStyle & ~(WS_CAPTION | WS_SIZEBOX));

	// Resizes our RuneScape window to the desired dimensions
	// without moving it or changing its Z-location.
	SetWindowPos(runescapeWindow, NULL, NULL, NULL, WIDTH, HEIGHT, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);


	// Saves our instance in a global variable for future use.
	hInst = hInstance;

	// Defines how our overlay should look. Built in function
	// calculates how much extra space to accomodate for our
	// desired client size.
	AdjustWindowRectEx(&adjustedRect, style, true, WS_EX_LAYERED);
	int width = adjustedRect.right - adjustedRect.left;
	int height = adjustedRect.bottom - adjustedRect.top;
	// Creates our overlay window with the specified styles.
	// Our window cannot be rezied, maximized nor minimized.
	// It is also set as a child of the original RuneScape window,
	// this is to fix Z-position rendering issues. Also displays
	// the client correctly when the user clicks the RuneScape
	// icon in the system tray. Asserts we actually created it.
	HWND hWnd = CreateWindowEx(WS_EX_LAYERED, szWindowClass, szTitle, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, runescapeWindow, NULL, hInstance, NULL);
	if (!hWnd) return FALSE;
	overlay = hWnd;

	// Defines (255, 0, 255) 'MAGENTA' as our transparent color.
	// This color (should) rarely be used and therefor is a good
	// fit for our task. Also sets the overlay background to
	// this color. (Rendering it transparent).
	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
	SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG) brush);
	SetLayeredWindowAttributes(hWnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
	if(DEBUG) DeleteObject(brush);
	// Shows and updates the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//   FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//   PURPOSE:  Processes messages for the main window.
//
//   WM_MOVE	- handle when window is moved
//   WM_COMMAND	- process the application menu
//   WM_PAINT   - paint the main window
//   WM_DESTROY - post a quit message and return
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// Variables used when repainting, declared here
	// to avoid GDI leaks.
	static HPEN transparent;
	static HPEN green;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
		case WM_CREATE: {
			// Create our GDI objects only once to avoid
			// GDI leaks.
			transparent	= CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
			green		= CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		}
		case WM_MOVE: {
			// Gets the current overlay position and calculates where
			// its client area begins. We then set the RuneScape window
			// to these coordinates, creating the illusion of a normal
			// window.
			RECT windowRect;
			GetWindowRect(hWnd, &windowRect);
			int x = windowRect.left + abs(adjustedRect.left);
			int y = windowRect.top + abs(adjustedRect.top);
			// We only want to update the position, not send any frame
			// repaints, Z-index change or resize it. (RuneScape window
			// should already be desired size from initilazation)
			SetWindowPos(runescapeWindow, NULL, x, y, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				case IDM_SCRIPT_LOAD: {
					// Opens a file dialog according to our specifications
					wchar_t fn[MAX_PATH] = { 0 };
					OPENFILENAME ofn = ScriptFiles(hWnd);
					ofn.lpstrFile = fn;
					GetOpenFileName(&ofn);

					// Translates the filename and checks whether we actually
					// did select a file. If so we set any running scripts
					// for termination and waits until they are terminated.
					// We then create a new thread and run our script.
					string script = w2s(fn);
					if (script.length() != 0) {
						if (scriptStatus == LUA_RUNNING) scriptStatus = LUA_STOPPING;
						while (scriptStatus != LUA_STOPPED) {}
						thread client(RunLuaScript, script);
						client.detach();
					}
					break;
				}
				case IDM_SCRIPT_STOP: {
					if (scriptStatus == LUA_RUNNING) scriptStatus = LUA_STOPPING;
				}
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		}
		case WM_PAINT: {
			hdc = BeginPaint(hWnd, &ps);

			// To speed up the painting process we use transparent
			// colors and simply redraw everything we drawed last
			// time. This effectivly clears the screen. Below we
			// clear the mouse crosshair
			SelectObject(hdc, transparent);
			MoveToEx(hdc, 0, pmy, (LPPOINT) NULL);
			LineTo(hdc, WIDTH, pmy);
			MoveToEx(hdc, pmx, 0, (LPPOINT) NULL);
			LineTo(hdc, pmx, HEIGHT);

			// Update our previous mouse position to the current
			// one. Used next painting cycle.
			pmy = my;
			pmx = mx;

			// Select the appropriate pen and draw the crosshair
			SelectObject(hdc, green);
			MoveToEx(hdc, 0, my, (LPPOINT) NULL);
			LineTo(hdc, WIDTH, my);
			MoveToEx(hdc, mx, 0, (LPPOINT) NULL);
			LineTo(hdc, mx, HEIGHT);

			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
			// Release our GDI objects to not cause leaks.
			DeleteObject(transparent);
			DeleteObject(green);

			// Enable user interaction incase we were running
			// a script. Also reset the RuneScape window style.
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
		case WM_COMMAND: {
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR) TRUE;
			}
			break;
		}
	}
	return (INT_PTR) FALSE;
}
