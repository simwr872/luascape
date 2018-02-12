#include "Mouse.h"
#include <iostream>

void Mouse::Click() {
	// Send a click message to the RuneScape client at
	// our current mouse coordinates with a delay before
	// we release the mouse. WParam must be set to
	// MK_LBUTTON (0x1) which marks that the left button
	// is being held
	PostMessage(client, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pos.x, pos.y));
	// TODO: lognormal distribution AND record how long clicks actually are.
	Sleep(rand() % (120 - 60 + 1) + 60);
	// WParam must be 0x0 which marks that no other key
	// is being pressed
	PostMessage(client, WM_LBUTTONUP, 0, MAKELPARAM(pos.x, pos.y));
}

void Mouse::Move(float x, float y, WPARAM wParam = 0) {
	pos = vec2(x, y);
	if ((int)pos.x != (int)prev.x || (int) pos.y != (int) prev.y) {
		PostMessage(client, WM_MOUSEMOVE, wParam, MAKELPARAM(pos.x, pos.y));
		InvalidateRect(overlay, NULL, NULL);
	}
}

float Mouse::MouseDist(float x) {
	float ts = pow(x, 2);
	float tc = pow(x, 3);
	return 1 - (tc*ts + -5 * tc + 5 * ts);
}

void Mouse::SmoothMove(float x, float y) {
	float time = 1000; // Change to separate function. Fitts law
	vec2 goal(x, y);
	vec2 difference = goal - pos;
	vec2 normal = goal.normal();
	vec2 current;
	float distance;
	for (float i = 0; i < time; i++) {
		int start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		distance = MouseDist(i / time);
		current = goal - difference*distance;
		Move(current.x, current.y);
		int end;
		do { end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); } while (end-start<1);
		if (end - start != 1) {
			i += (end - start) - 1;
		}
	}
	Move(current.x, current.y);
}


vec2 Mouse::FindMove(float x, float y, std::string find, Screen& screen) {
	float time = 3000; // Change to separate function. Fitts law
	vec2 goal(x, y);
	vec2 difference = goal - pos;
	vec2 normal = goal.normal();
	vec2 current;
	float distance;
	for (float i = 0; i < time; i++) {
		int start = std::chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if (screen.ReadHover().find(find) != std::string::npos) {
			return current;
		}
		distance = MouseDist(i / time);
		current = goal - difference*distance;
		Move(current.x, current.y);
		int end;
		do { end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); } while (end - start<1);
		if (end - start != 1) {
			i += (end - start) - 1;
		}
	}
	return vec2();
}


void Mouse::Paint(HDC hdc, HPEN transparent, HPEN green) {
	// To speed up the painting process we use transparent
	// colors and simply redraw everything we drawed last
	// time. This effectivly clears the screen. Below we
	// clear the mouse crosshair
	SelectObject(hdc, transparent);
	MoveToEx(hdc, 0, prev.y, (LPPOINT) NULL);
	LineTo(hdc, 800, prev.y);
	MoveToEx(hdc, prev.x, 0, (LPPOINT) NULL);
	LineTo(hdc, prev.x, 600);

	// Update our previous mouse position to the current
	// one. Used next painting cycle.
	prev = pos;

	// Select the appropriate pen and draw the crosshair
	SelectObject(hdc, green);
	MoveToEx(hdc, 0, pos.y, (LPPOINT) NULL);
	LineTo(hdc, 800, pos.y);
	MoveToEx(hdc, pos.x, 0, (LPPOINT) NULL);
	LineTo(hdc, pos.x, 600);
}

Mouse::Mouse(HWND _client, HWND _overlay) {
	client = _client;
	overlay = _overlay;
}
