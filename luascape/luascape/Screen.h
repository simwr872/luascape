#pragma once
#include <Windows.h>
#include <string>
#include <bitset>
#include <map>
#include <algorithm>
class Screen {
private:
	static const std::map<int, std::bitset<108>> ascii;
	static const std::map<int, std::bitset<108>> GenerateMap();
	HWND client;
	BITMAPINFO CreateBitmap(int x, int y);
	float Brightness(BYTE * bitPointer, int x, int y, int width);
	char IdentifyCharacter(std::bitset<108> data);
	bool NextCharacter(BYTE * bitPointer, int & x, int y, int width);
	std::bitset<108> GetCharacter(BYTE * bitPointer, int & x, const int y, int width);
	std::string FindCharacters(BYTE * bitPointer, int width);
public:
	void ReadPixel(int x, int y, int * color);
	std::string ReadChat(int line);
	Screen(HWND _client);
	Screen() {};
	~Screen() {};
};

