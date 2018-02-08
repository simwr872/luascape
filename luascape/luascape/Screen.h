#pragma once
#include <Windows.h>
#include <string>
#include <bitset>
#include <map>
#include <algorithm>
class Screen {
private:
	HWND client;
	static const int CHAR_WIDTH = 9;
	static const int CHAR_HEIGHT = 7;
	static const COLORREF colors[];
	static const std::map<char, long long> chars;
	static std::map<char, long long> GenerateMap();

	BITMAPINFO CreateBitmap(const int x, const int y);
	COLORREF Color(const BYTE * bitPointer, const int x, const int y, const int width);
	bool Match(const COLORREF & color);
	bool NextCharacter(const BYTE * bitPointer, int & _x, const int _y, const int width);
	long long GetCharacter(const BYTE * bitPointer, int & _x, const int _y, const int width);
	char SolveClash(const BYTE * bitPointer, const int x, const char c, const int width);
	char IdentifyCharacter(const BYTE * bitPointer, const int x, const long long c, const int width);


public:
	std::string ReadHover();
	COLORREF ReadPixel(const int x, const int y);
	Screen(HWND _client);
	Screen() {};
	~Screen() {};
};

