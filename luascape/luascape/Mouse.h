#pragma once
#include <Windows.h>
#include "Vec2.h"
#include <chrono>
#include <string>
#include "Screen.h"
#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define FITTS_A 100
#define FITTS_B 200
class Mouse {
private:
	HWND client;
	HWND overlay;
	vec2 prev;
	vec2 pos;
	float MouseDist(float x);
	int FittsLaw(const float distance, const int size);
	vec2 Vec2Lerp(vec2 from, vec2 to, float percent);
	void Move(const int x, const int y, const WPARAM wParam);
	float MouseDeviation(vec2 p1, vec2 p2, float x);
public:
	void Click(int button);
	void SmoothMove(const int x, const int y, const int size);
	void PaintClear(const HDC & hdc, const HPEN & transparent);
	void Paint(const HDC& hdc, const HPEN& color);
	Mouse(HWND _client, HWND _overlay);
	Mouse() {};
	~Mouse() {};
};

