#pragma once
#include <Windows.h>
#include "Vec2.h"
#include <chrono>
class Mouse {
private:
	HWND client;
	HWND overlay;
	vec2 prev;
	vec2 pos;
	float MouseDist(float x);
public:
	void Click();
	void Move(float x, float y, WPARAM wParam);
	void SmoothMove(float x, float y);
	void Paint(HDC hdc, HPEN transparent, HPEN green);
	Mouse(HWND _client, HWND _overlay);
	Mouse() {};
	~Mouse() {};
};

