#include "Screen.h"
#include <iostream>

const std::map<int, std::bitset<108>> Screen::ascii = Screen::GenerateMap();
const std::map<int, std::bitset<108>> Screen::GenerateMap() {
	std::map<int, std::bitset<108>> chars;

	chars[65] = std::bitset<108>("000000001000000001111000000111100000011100100000011110100000000011110000000000011000000000000000000000000000");
	chars[66] = std::bitset<108>("011111111000010010001000010010001000010010001000010111011000001101110000000000000000000000000000000000000000");
	chars[67] = std::bitset<108>("000111100000001100110000011000011000010000001000010000001000010000001000011000011000000000010000000000000000");
	chars[68] = std::bitset<108>("011111111000010000001000010000001000010000001000010000001000001100110000000111100000000000000000000000000000");
	chars[69] = std::bitset<108>("011111111000010001001000010001001000010001001000010000001000000000000000000000000000000000000000000000000000");
	chars[70] = std::bitset<108>("011111111000010001000000010001000000010001000000010000000000000000000000000000000000000000000000000000000000");
	chars[71] = std::bitset<108>("000111100000001100110000011000001000010000001000010000001000010001001000010001011000000001111000000000000000");
	chars[72] = std::bitset<108>("011111111000000001000000000001000000000001000000000001000000000001000000011111111000000000000000000000000000");
	chars[73] = std::bitset<108>("010000001000011111111000010000001000000000000000000000000000000000000000000000000000000000000000000000000000");
	chars[74] = std::bitset<108>("000000110000000000011000010000001000010000011000011111110000000000000000000000000000000000000000000000000000");
	chars[75] = std::bitset<108>("011111111000000010000000000011000000001101100000011000011000010000001000000000000000000000000000000000000000");
	chars[76] = std::bitset<108>("011111111000000000001000000000001000000000001000000000001000000000000000000000000000000000000000000000000000");
	chars[77] = std::bitset<108>("011111111000001100000000000011000000000001110000000001110000000011000000001100000000011111111000000000000000");
	chars[78] = std::bitset<108>("011111111000011100000000000110000000000011000000000001100000000000111000011111111000000000000000000000000000");
	chars[79] = std::bitset<108>("000111100000001100110000011000011000010000001000010000001000010000001000011000011000001100110000000111000000");
	chars[80] = std::bitset<108>("011111111000010001000000010001000000010001000000011011000000001110000000000000000000000000000000000000000000");
	chars[81] = std::bitset<108>("000111100000001100110000011000001000010000001000010000001000010000101000011000011000000111111000000000000000");
	chars[82] = std::bitset<108>("011111111000010001000000010001000000010001100000011110011000000000001000000000000000000000000000000000000000");
	chars[83] = std::bitset<108>("001100010000011010001000010010001000010001001000010001111000000000000000000000000000000000000000000000000000");
	chars[84] = std::bitset<108>("010000000000010000000000010000000000011111111000010000000000010000000000010000000000000000000000000000000000");
	chars[85] = std::bitset<108>("011111110000000000011000000000001000000000001000000000001000011111110000000000000000000000000000000000000000");
	chars[86] = std::bitset<108>("011100000000000111000000000000111000000000111000000111100000011100000000010000000000000000000000000000000000");
	chars[87] = std::bitset<108>("011100000000000111110000000000111000001111100000011100000000001111100000000000111000000111110000011110000000");
	chars[88] = std::bitset<108>("010000001000011000111000000111100000000111000000001100110000010000011000000000000000000000000000000000000000");
	chars[89] = std::bitset<108>("010000000000011100000000000111000000000011111000001110000000011000000000000000000000000000000000000000000000");
	chars[90] = std::bitset<108>("010000001000010000111000010001101000010110001000011100001000010000001000000000000000000000000000000000000000");

	chars[97] = std::bitset<108>("000000110000000101101000000101001000000101001000000111111000000000000000000000000000000000000000000000000000");
	chars[98] = std::bitset<108>("011111111000000110011000000100001000000100001000000110011000000011110000000000000000000000000000000000000000");
	chars[99] = std::bitset<108>("000001110000000110011000000100001000000100001000000100001000000000000000000000000000000000000000000000000000");
	chars[100] = std::bitset<108>("000011110000000110011000000100001000000100001000000110011000011111111000000000000000000000000000000000000000");
	chars[101] = std::bitset<108>("000011100000000111011000000101001000000101001000000111001000000011000000000000000000000000000000000000000000");
	chars[102] = std::bitset<108>("000100000000011111111000010100000000000000000000000000000000000000000000000000000000000000000000000000000000");
	chars[103] = std::bitset<108>("000011110000000110011011000100001001000100001001000110011011000111111100000000000000000000000000000000000000");
	chars[104] = std::bitset<108>("011111111000000110000000000100000000000100000000000111111000000000000000000000000000000000000000000000000000");
	chars[105] = std::bitset<108>("010111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	chars[106] = std::bitset<108>("000000000001000000000011010111111110000000000000000000000000000000000000000000000000000000000000000000000000");
	chars[107] = std::bitset<108>("011111111000000001000000000011100000000110011000000100001000000000000000000000000000000000000000000000000000");
	chars[108] = std::bitset<108>("011111111000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	chars[109] = std::bitset<108>("000111111000000110000000000100000000000100000000000111111000000110000000000100000000000100000000000011111000");
	chars[110] = std::bitset<108>("000111111000000110000000000100000000000100000000000100000000000011111000000000000000000000000000000000000000");
	chars[111] = std::bitset<108>("000001100000000110011000000100001000000100001000000100001000000110111000000001100000000000000000000000000000");
	chars[112] = std::bitset<108>("000111111110000100001000000100001000000100001000000011110000000000000000000000000000000000000000000000000000");
	chars[113] = std::bitset<108>("000011110000000110011000000100001000000100001000000111111110000000000000000000000000000000000000000000000000");
	chars[114] = std::bitset<108>("000111111000000110000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000");
	chars[115] = std::bitset<108>("000010001000000101001000000101101000000100101000000100010000000000000000000000000000000000000000000000000000");
	chars[116] = std::bitset<108>("000100000000001111110000000100001000000100001000000000000000000000000000000000000000000000000000000000000000");
	chars[117] = std::bitset<108>("000111110000000000001000000000001000000000001000000111111000000000000000000000000000000000000000000000000000");
	chars[118] = std::bitset<108>("000110000000000011110000000000011000000001110000000110000000000000000000000000000000000000000000000000000000");
	chars[119] = std::bitset<108>("000100000000000111100000000000111000000001111000000111000000000111100000000000111000000001111000000111000000");
	chars[120] = std::bitset<108>("000100001000000110011000000001100000000011110000000100001000000000000000000000000000000000000000000000000000");
	chars[121] = std::bitset<108>("000110000001000001110011000000011110000001110000000111000000000000000000000000000000000000000000000000000000");
	chars[122] = std::bitset<108>("000100001000000100011000000101101000000111001000000110001000000000000000000000000000000000000000000000000000");

	chars[49] = std::bitset<108>("011000001000011111111000000000001000000000001000000000000000000000000000000000000000000000000000000000000000");
	chars[50] = std::bitset<108>("011000011000010000111000010001101000011111001000001100000000000000000000000000000000000000000000000000000000");
	chars[51] = std::bitset<108>("010000001000010010001000010011001000011111111000000000110000000000000000000000000000000000000000000000000000");
	chars[52] = std::bitset<108>("000001100000000011100000001100100000011000100000011111111000000000100000000000000000000000000000000000000000");
	chars[53] = std::bitset<108>("011110001000010010001000010010001000010011110000000000000000000000000000000000000000000000000000000000000000");
	chars[54] = std::bitset<108>("001111110000011010011000010010001000010010001000010001110000000000000000000000000000000000000000000000000000");
	chars[55] = std::bitset<108>("010000000000010000011000010001110000010111000000011000000000000000000000000000000000000000000000000000000000");
	chars[56] = std::bitset<108>("011101111000010011001000010010001000010111001000001101110000000000000000000000000000000000000000000000000000");
	chars[57] = std::bitset<108>("011111001000010001001000010001001000011001011000001111100000000000000000000000000000000000000000000000000000");
	chars[48] = std::bitset<108>("001111110000011000001000010000001000010000001000001111110000000000000000000000000000000000000000000000000000");
	chars[33] = std::bitset<108>("011111101000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	chars[63] = std::bitset<108>("010000000000010001101000010011000000011110000000000000000000000000000000000000000000000000000000000000000000");
	chars[44] = std::bitset<108>("000000000100000000011000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	chars[46] = std::bitset<108>("000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	chars[39] = std::bitset<108>("011000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	return chars;
}

BITMAPINFO Screen::CreateBitmap(int x, int y) {
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = x;
	bitmap.bmiHeader.biHeight = -y;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = x * 4 * y;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	return bitmap;
}

float Screen::Brightness(BYTE* bitPointer, int x, int y, int width) {
	int index = y * width + x;
	int blue = (int) bitPointer[index * 4 + 0];
	int green = (int) bitPointer[index * 4 + 1];
	int red = (int) bitPointer[index * 4 + 2];
	return max(red, max(blue, green)) / 255.0;
}


int compare(std::bitset<108> a, std::bitset<108> b) {
	int score = 0;
	for (int i = 0; i < 108; i++) {
		if (a[i] != b[i]) score--;
		if (score < -8) break;
	}
	return score;
}

char Screen::IdentifyCharacter(std::bitset<108> data) {
	int score = -8;
	char c = '.';
	for (std::map<int, std::bitset<108>>::const_iterator i = Screen::ascii.begin(); i != Screen::ascii.end(); i++) {
		int ns = compare(i->second, data);
		if (ns > score) {
			score = ns;
			c = i->first;
		}
		//reversed storage
	}

	return c;
}

bool Screen::NextCharacter(BYTE* bitPointer, int& _x, int _y, int width) {
	// Iterate pixels until we find a bright pixel. Travel at most
	// 6 pixels since spaces are 5 pixels wide.
	for (int x = _x; x < _x + 6; x++) {
		for (int y = _y; y < _y + 12; y++) {
			if (Brightness(bitPointer, x, y, width) > 0.4) {
				_x = x;
				return true;
			}
		}
	}
	return false;
}

std::bitset<108> Screen::GetCharacter(BYTE* bitPointer, int& _x, const int _y, int width) {
	std::bitset<108> data(0);
	bool found = false;
	for (int x = _x; x < _x + 10; x++) {
		for (int y = _y; y < _y + 12; y++) {
			if (Brightness(bitPointer, x, y, width) > 0.4) {
				data.set(107 - ((x - _x)*12 + y), true);
				found = true;
			}
		}
		if (!found) {
			_x = x;
			return data;
		}
		found = false;
	}
}

std::string Screen::FindCharacters(BYTE* bitPointer, int width) {
	std::string s = "";
	int x = 0;
	int px = 0;
	std::bitset<108> data(0);

	while (x < width-15) {
		data.reset();
		px = x;
		if (!NextCharacter(bitPointer, x, 0, width)) break;
		if (x - px > 3) s += ' ';
		data = GetCharacter(bitPointer, x, 0, width);
		s += IdentifyCharacter(data);
	}
	return s;
}


std::string Screen::ReadChat(int line) {
	int lineOffsetX = 4;
	int lineOffsetY = 540 - 14 * (line - 1);
	int lineWidth = 526;
	int lineHeight = 12;
	// Define the bitmap. Height is inverted to make
	// the origin top-left instead of bitmaps default
	// bottom-left. 32 bitcount for all colors.
	BITMAPINFO bitmap = CreateBitmap(lineWidth, lineHeight);

	// Create and associate a bitpointer to the bits of
	// of our bitmap. We then select our bitmap and
	// copy all of RuneScape's buffer into ours.
	BYTE* bitPointer;

	// Fetch the RuneScape client device context and
	// create another one compatible with it. We must
	// not occupy the context for too long. (blocking)
	HDC deviceContext = GetDC(client);
	HDC deviceContextCopy = CreateCompatibleDC(deviceContext);

	HBITMAP hBitmap = CreateDIBSection(deviceContextCopy, &bitmap, DIB_RGB_COLORS, (void**) &bitPointer, NULL, NULL);
	SelectObject(deviceContextCopy, hBitmap);
	BitBlt(deviceContextCopy, 0, 0, lineWidth, lineHeight, deviceContext, lineOffsetX, lineOffsetY, SRCCOPY);

	// Immediatly release RuneScape's device context
	ReleaseDC(client, deviceContext);
	DeleteObject(hBitmap);


	std::string s = FindCharacters(bitPointer, lineWidth);

	// Delete any context we created.
	DeleteDC(deviceContextCopy);

	return s;
}


//
//   FUNCTION: ReadPixel()
//
//   PURPOSE:  Reads a pixel color from the RuneScape client.
//
void Screen::ReadPixel(int x, int y, int* color) {
	// Define the bitmap. Height is inverted to make
	// the origin top-left instead of bitmaps default
	// bottom-left. 32 bitcount for all colors.
	BITMAPINFO bitmap = CreateBitmap(1, 1);

	// Create and associate a bitpointer to the bits of
	// of our bitmap. We then select our bitmap and
	// copy all of RuneScape's buffer into ours.
	BYTE* bitPointer;

	// Fetch the RuneScape client device context and
	// create another one compatible with it. We must
	// not occupy the context for too long. (blocking)
	HDC deviceContext = GetDC(client);
	HDC deviceContextCopy = CreateCompatibleDC(deviceContext);

	HBITMAP hBitmap = CreateDIBSection(deviceContextCopy, &bitmap, DIB_RGB_COLORS, (void**) &bitPointer, NULL, NULL);
	SelectObject(deviceContextCopy, hBitmap);
	BitBlt(deviceContextCopy, 0, 0, 1, 1, deviceContext, 0, 0, SRCCOPY);

	// Immediatly release RuneScape's device context
	ReleaseDC(client, deviceContext);
	DeleteObject(hBitmap);

	// Parse the colors, each pixel has 4 bytes of
	// information. (the 4th being alpha)
	int index = 0 * 1 + 0;//y*w+x
	int blue = (int) bitPointer[index * 4 + 0];
	int green = (int) bitPointer[index * 4 + 1];
	int red = (int) bitPointer[index * 4 + 2];
	color[0] = red;
	color[1] = green;
	color[2] = blue;

	// Delete any context we created.
	DeleteDC(deviceContextCopy);
}



Screen::Screen(HWND _client) {
	client = _client;
}