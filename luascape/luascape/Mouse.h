#pragma once
#include <Windows.h>
#include "Vec2.h"
#include <chrono>
#include <string>
#include "Screen.h"
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
	vec2 FindMove(float x, float y, std::string find, Screen& screen);
	void Paint(HDC hdc, HPEN transparent, HPEN green);
	Mouse(HWND _client, HWND _overlay);
	Mouse() {};
	~Mouse() {};
};

