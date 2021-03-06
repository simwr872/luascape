#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <algorithm>
#define CHAR_WIDTH 9
#define CHAR_HEIGHT 7
class Screen {
private:
	HWND client;
	static const COLORREF colors[];
	static const std::map<char, long long> chars;
	static std::map<char, long long> GenerateMap();

	COLORREF Color(const BYTE * bitPointer, const int x, const int y, const int width);
	bool Match(const COLORREF & color);
	bool NextCharacter(const BYTE * bitPointer, int & _x, const int _y, const int width);
	long long GetCharacter(const BYTE * bitPointer, int & _x, const int _y, const int width);
	char SolveClash(const BYTE * bitPointer, const int x, const char c, const int width);
	char IdentifyCharacter(const BYTE * bitPointer, const int x, const long long c, const int width);


public:
	std::string ReadHover();
	void CaptureArea(BYTE * bitPointer, const int x, const int y, const int w, const int h);
	Screen(HWND _client);
	Screen() {};
	~Screen() {};
};

