// luascape.cpp : Defines the entry point for the application.
//

#include <Windows.h>
#include "StringHelp.h"
#include <commdlg.h>
#include <iostream>
#include "luascape.h"
#include "lua.hpp"
#include <thread>
#include <chrono>
#include "Mouse.h"
#include "Keyboard.h"
#include "Screen.h"
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

Keyboard keyboard;
Mouse mouse;
Screen screen;

static const int WIDTH = 800;
static const int HEIGHT = 600;
HWND runescapeWindow;
HWND runescapeClient;
long runescapeStyle;
int scriptStatus = LUA_STOPPED;
float mx, my, pmx, pmy;							// Previous and current mouse x & y


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
	string dir = StringHelp::w2s(path);
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
	/*pwd.erase(pwd.rfind('\\'));
	pwd.erase(pwd.rfind('\\'));
	pwd.erase(pwd.rfind('\\'));*/
	pwd += "\\scripts";
	//TODO: PREPEND '/script' DIRECTORY

	ofn.lpstrInitialDir = StringHelp::s2w(pwd.c_str());
	ofn.lpstrFilter = L"Lua files (.lua)\0*.LUA";
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	return ofn;
}

//
//   FUNCTION: LuaMove(lua_State *Lua)
//
//   PURPOSE:  Allows the script to move the mouse inside the RuneScape client.
//
int LuaMove(lua_State *Lua) {
	// Fetch both coordinates and assert that they
	// are a number type and also in a valid range.
	int x = lua_tonumber(Lua, -3);
	if(lua_type(Lua, -3) != LUA_TNUMBER) luaL_argerror(Lua, 1, "Expected number.");
	if(x < 0 || x > WIDTH - 1) luaL_argerror(Lua, 1, "Out of bounds.");

	int y = lua_tonumber(Lua, -2);
	if (lua_type(Lua, -2) != LUA_TNUMBER) luaL_argerror(Lua, 2, "Expected number.");
	if (y < 0 || y > HEIGHT - 1) luaL_argerror(Lua, 2, "Out of bounds.");

	int size = lua_tonumber(Lua, -1);
	if (lua_type(Lua, -1) != LUA_TNUMBER) luaL_argerror(Lua, 3, "Expected number.");
	if (size < 0 || size > max(HEIGHT, WIDTH)) luaL_argerror(Lua, 3, "Out of bounds.");

	// Pop the 2 values from the stack for cleanup.
	lua_pop(Lua, 3);

	mouse.SmoothMove(x, y, size);

	return 0;
}


//
//   FUNCTION: LuaPress(lua_State *Lua)
//
//   PURPOSE:  Allows the script to press keys in RuneScape client.
//
//   COMMENTS:
//
//        Function will handle both ascii numbers and strings.
//
int LuaPress(lua_State *Lua) {
	int type = lua_type(Lua, -1);
	char c;
	string s;
	// Assert that the type is either a string
	// or a number and convert to a char.
	switch (lua_type(Lua, -1)) {
		case LUA_TNUMBER:
			c = lua_tonumber(Lua, -1);
			break;
		case LUA_TSTRING:
			s = lua_tostring(Lua, -1);
			c = s.at(0);
			break;
		default:
			luaL_argerror(Lua, 1, "Expected string or number.");
	}
	lua_pop(Lua, 1);

	if (!keyboard.PressKey(c)) luaL_error(Lua, ("Character '"+to_string(c)+"' not yet implemented!").c_str());

	return 0;
}


//
//   FUNCTION: LuaHover(lua_State *Lua)
//
//   PURPOSE:  Allows the script to read RuneScapes hover text.
//
int LuaHover(lua_State *Lua) {
	// We push the result to the stack and return
	// 1 to indicate 1 return value.
	lua_pushstring(Lua, screen.ReadHover().c_str());
	return 1;
}

//
//   FUNCTION: LuaClick(lua_State *Lua)
//
//   PURPOSE:  Allows the script to click the RuneScape client.
//
int LuaClick(lua_State *Lua) {
	int button;
	if (lua_gettop(Lua) < 1) {
		button = MOUSE_LEFT;
	} else if (lua_type(Lua, -1) != LUA_TNUMBER) {
		luaL_argerror(Lua, 1, "Expected number.");
	} else {
		button = lua_tonumber(Lua, -1);
		cout << button << endl;
		if (button != MOUSE_LEFT && button != MOUSE_RIGHT) luaL_argerror(Lua, 1, "Button must be left (1) or right (2).");
	}

	mouse.Click(button);
	return 0;
}

//
//   FUNCTION: LuaCapture(lua_State *Lua)
//
//   PURPOSE:  Allows the script to read pixels from the RuneScape client.
//
int LuaCapture(lua_State *Lua) {
	// Fetch all parameters and assert that they
	// are the correct type and within bounds.
	float x = lua_tonumber(Lua, -4);
	if (lua_type(Lua, -2) != LUA_TNUMBER) luaL_argerror(Lua, 1, "Expected number.");
	if (x < 0 || x > WIDTH - 1) luaL_argerror(Lua, 1, "Out of bounds.");

	float y = lua_tonumber(Lua, -3);
	if (lua_type(Lua, -1) != LUA_TNUMBER) luaL_argerror(Lua, 2, "Expected number.");
	if (x < 0 || x > HEIGHT - 1) luaL_argerror(Lua, 2, "Out of bounds.");

	float w = lua_tonumber(Lua, -2);
	if (lua_type(Lua, -2) != LUA_TNUMBER) luaL_argerror(Lua, 3, "Expected number.");
	if (w < 0 || w > WIDTH || x + w > WIDTH) luaL_argerror(Lua, 3, "Out of bounds.");

	float h = lua_tonumber(Lua, -1);
	if (lua_type(Lua, -1) != LUA_TNUMBER) luaL_argerror(Lua, 4, "Expected number.");
	if (h < 0 || h > HEIGHT || y + h > HEIGHT) luaL_argerror(Lua, 4, "Out of bounds.");

	// Pop the variables from the stack to cleanup
	lua_pop(Lua, 4);

	BYTE* bitPointer = (BYTE*) malloc(4 * w*h);
	screen.CaptureArea(bitPointer, x, y, w, h);

	// Colors are stored as a decimal value in tables
	// that can be accessed from within the script as
	// t[x][y]. Furthermore to separate r, g, b values
	// one must shift 16, 8 and 0 bits respectivly.
	lua_newtable(Lua);
	for (int _x = 0; _x < w; _x++) {
		// Since lua tables start at index 1 we must
		// offset by one.
		lua_pushnumber(Lua, _x + 1);
		lua_newtable(Lua);

		for (int _y = 0; _y < h; _y++) {
			lua_pushnumber(Lua, _y+1);

			int index	= _y*w + _x;
			int blue	= (int) bitPointer[index * 4 + 0];
			int green	= (int) bitPointer[index * 4 + 1] << 8;
			int red		= (int) bitPointer[index * 4 + 2] << 16;

			lua_pushnumber(Lua, red + green + blue);
			lua_settable(Lua, -3);
		}
		lua_settable(Lua, -3);
	}
	delete bitPointer;

	// We are returning one value (the (perhaps big) table)
	return 1;
}

static const luaL_Reg LuaLibrary[] = {
	{ "click", LuaClick },
	{ "hover", LuaHover },
	{ "move", LuaMove },
	{ "press", LuaPress },
	{ "capture", LuaCapture },
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
		MessageBox(NULL, StringHelp::s2w(s.c_str()), L"Lua status", MB_OK);
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
	keyboard = Keyboard(runescapeClient);
	mouse = Mouse(runescapeClient, hWnd);
	screen = Screen(runescapeClient);

	// Defines (255, 0, 255) 'MAGENTA' as our transparent color.
	// This color (should) rarely be used and therefor is a good
	// fit for our task. Also sets the overlay background to
	// this color. (Rendering it transparent).
	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
	SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG) brush);
	SetLayeredWindowAttributes(hWnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
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
					string script = StringHelp::w2s(fn);
					if (script.length() != 0) {
						if (scriptStatus == LUA_RUNNING) scriptStatus = LUA_STOPPING;
						while (scriptStatus != LUA_STOPPED) {}
						// Declare the script state as running
						scriptStatus = LUA_RUNNING;
						thread client(RunLuaScript, script);
						client.detach();
						InvalidateRect(hWnd, 0, 0);
					}
					break;
				}
				case IDM_SCRIPT_STOP: {
					if (scriptStatus == LUA_RUNNING) scriptStatus = LUA_STOPPING;
					InvalidateRect(hWnd, 0, 0);
				}
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		}
		case WM_PAINT: {
			hdc = BeginPaint(hWnd, &ps);

			// We can always clear the cursor but only draw during script.
			mouse.PaintClear(hdc, transparent);
			if (scriptStatus == LUA_RUNNING) mouse.Paint(hdc, green);

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
