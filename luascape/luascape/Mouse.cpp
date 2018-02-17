#include "Mouse.h"
#include <iostream>
#include <random>
#include <cmath>

void Mouse::Click(int button) {
	// Send a click message to the RuneScape client at
	// our current mouse coordinates with a delay before
	// we release the mouse. WParam must be set to
	// MK_LBUTTON (0x1) which marks that the left button
	// is being held OR (0x2) which marks right button.
	// The msg is either 201 and later 202 for left
	// up/down. Or 204 and 205 for the right button.
	int bs = std::pow(button, 2);
	PostMessage(client, 0x0200 | bs, button, MAKELPARAM(pos.x, pos.y));
	// TODO: lognormal distribution AND record how long clicks actually are.
	Sleep(rand() % (120 - 60 + 1) + 60);
	// WParam must be 0x0 which marks that no other key
	// is being pressed
	PostMessage(client, (0x0200 | bs) + 1, 0, MAKELPARAM(pos.x, pos.y));
}

void Mouse::Move(const int x, const int y, const WPARAM wParam = 0) {
	pos = vec2(x, y);
	// We only actually update the mouse if the real
	// position has changed. Since it is represented
	// as floats but pixels are not divisible.
	if (pos.x != prev.x || pos.y != prev.y) {
		PostMessage(client, WM_MOUSEMOVE, wParam, MAKELPARAM(pos.x, pos.y));
		// Invalidate our window to schedule a redraw.
		InvalidateRect(overlay, NULL, NULL);
	}
}

//
//   FUNCTION: MouseDist(float x)
//
//   PURPOSE:  A function that describes mouse movement.
//
//   COMMENTS:
//
//        http://www.timotheegroleau.com/Flash/experiments/easing_function_generator.htm
//        Function must satisfy f(0) = 0 and f(1) = 1. Ease
//        functions do just this, an easy tool is linked.
//
float Mouse::MouseDist(float x) {
	float ts = pow(x, 2);
	float tc = pow(x, 3);
	return tc*ts + -5 * tc + 5 * ts;
}

//
//   FUNCTION: FittsLaw(const float distance, const int size)
//
//   PURPOSE:  Returns how many milliseconds a mouse movement
//             should take.
//
int Mouse::FittsLaw(const float distance, const int size) {
	return FITTS_A + FITTS_B * log2(distance / size + 1);
}

//
//   FUNCTION: Vec2Lerp(vec2 from, vec2 to, float percent)
//
//   PURPOSE:  Lerps a vector.
//
vec2 Mouse::Vec2Lerp(vec2 from, vec2 to, float percent) {
	return from*(1.0 - percent) + to*percent;
}

//
//   FUNCTION: MouseDeviation(vec2 p1, vec2 p2, float x)
//
//   PURPOSE:  A function that describes mouse movement.
//
//   COMMENTS:
//
//        https://denisrizov.com/2016/06/02/bezier-curves-unity-package-included/
//        A quadratic bezier curve that describes the mouse
//        deviation from the optimal path. The two points
//        as arguments must be within 0<x<1. The y value
//        of this bezier curve is then multiplied with some
//        value and the normal of the optimal path.
//
float Mouse::MouseDeviation(vec2 p1, vec2 p2, float x) {
	// TODO: err checking
	vec2 p0(0, 0);
	vec2 p3(1, 0);
	vec2 a = Vec2Lerp(p0, p1, x);
	vec2 b = Vec2Lerp(p1, p2, x);
	vec2 c = Vec2Lerp(p2, p3, x);
	vec2 d = Vec2Lerp(a, b, x);
	vec2 e = Vec2Lerp(b, c, x);
	vec2 p = Vec2Lerp(d, e, x);
	return p.y;
}

double doubleRand(double min, double max) {
	thread_local std::mt19937 generator(std::random_device {}());
	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(generator);
}

int Milliseconds() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Mouse::SmoothMove(const int x, const int y, const int size) {
	vec2 goal(x, y);
	vec2 start = pos;
	float distance = (goal - start).length();
	int time = FittsLaw(distance, size);
	vec2 normal = (goal - start).normal().normalize();
	vec2 p1(doubleRand(0.2, 0.4), doubleRand(-1, 1));
	vec2 p2(doubleRand(0.6, 0.8), doubleRand(-1, 1));
	vec2 current;
	float progress;
	for (float i = 0; i < time; i++) {
		int timeStart = Milliseconds();
		progress = MouseDist(i / time);
		current = Vec2Lerp(start, goal, progress);
		current = current + normal*MouseDeviation(p1, p2, progress)*(0.05*distance + 2);
		Move(current.x, current.y);
		int end; // TODO: move busy waiting to another class (maybe?)
		do { end = Milliseconds(); } while (end - timeStart<1);
		if (end - timeStart != 1) {
			i += (end - timeStart) - 1;
		}
	}
	Move(current.x, current.y);
}


void Mouse::PaintClear(const HDC& hdc, const HPEN& transparent) {
	// To speed up the painting process we use transparent
	// colors and simply redraw everything we drawed last
	// time. This effectivly clears the screen. Below we
	// clear the mouse crosshair
	SelectObject(hdc, transparent);
	MoveToEx(hdc, 0, prev.y, (LPPOINT) NULL);
	LineTo(hdc, 800, prev.y);
	MoveToEx(hdc, prev.x, 0, (LPPOINT) NULL);
	LineTo(hdc, prev.x, 600);
}


void Mouse::Paint(const HDC& hdc, const HPEN& color) {
	// Update our previous mouse position to the current
	// one. Used next painting cycle.
	prev = pos;

	// Select the appropriate pen and draw the crosshair
	SelectObject(hdc, color);
	MoveToEx(hdc, 0, pos.y, (LPPOINT) NULL);
	LineTo(hdc, 800, pos.y);
	MoveToEx(hdc, pos.x, 0, (LPPOINT) NULL);
	LineTo(hdc, pos.x, 600);
}

Mouse::Mouse(const HWND _client, const HWND _overlay) {
	client = _client;
	overlay = _overlay;
}
