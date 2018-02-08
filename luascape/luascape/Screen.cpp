#include "Screen.h"
#include <iostream>

const COLORREF Screen::colors[] = {
	RGB(248, 213, 107),	// Some items orange tint
	RGB(255, 152, 31),	// Withdrawing bank items
	RGB(255, 255, 255), // White
	RGB(0, 255, 255),	// Cyan, eg. Trees
	RGB(255, 255, 0),	// Yellow, npc/fishing spot
	RGB(184, 209, 209)	// Blueish white, items in bag
};
const std::map<char, long long> Screen::chars = Screen::GenerateMap();
std::map<char, long long> Screen::GenerateMap() {
	// Characters 1IlTij, area within >> are saved. The rest
	// is used to verify clashing characters. This way any
	// character can be stored within 63 bits.
	//   xx    xxxx   xx   xxxxxx
	//  xxx     xx    xx     xx
	//   xx     xx    xx     xx
	//>  xx     xx    xx     xx     xx   xx
	//   xx     xx    xx     xx
	//   xx     xx    xx     xx     xx   xx
	//   xx     xx    xx     xx     xx   xx
	//   xx     xx    xx     xx     xx   xx
	//   xx     xx    xx     xx     xx   xx
	//> xxxx   xxxx   xx     xx     xx   xx
	//                                   xx

	// 1 = I
	// T = l
	// i = j
	// r next to t will join and produce mutant character
	std::map<char, long long> chars;
	chars['a'] = 0b0000000000000000000000111110011111101001010100101011111100110100;
	chars['b'] = 0b0000000000000000000000011110011111101000010100001011111111111111;
	chars['c'] = 0b0000000000000000000000000000001001001000010100001011111100111100;
	chars['d'] = 0b0000000000000000000000111111111111111000010100001011111100111100;
	chars['e'] = 0b0000000000000000000000101110010111101010010101001011111100111100;
	chars['f'] = 0b0000000000000000000000000000100000010001000111111111111110001000;
	chars['g'] = 0b0000000000000000000000111110011111101000010100001011111100111100;
	chars['h'] = 0b0000000000000000000000111110011111100000010000001011111111111111;
	chars['i'] = 0b0000000000000000000000000000000000000000000000000011111011111101;
	chars['j'] = 0b0000000000000000000000000000000000000000000000000011111011111101;
	chars['k'] = 0b0000000000000000000000100000011000100110110001110011111111111111;
	chars['l'] = 0b0000000000000000000000000000000000000000000000000011111111111111;
	chars['m'] = 0b0000000011111001111110000001011111001111110000001011111101111110;
	chars['n'] = 0b0000000000000000000000111110011111100000010000001011111101111110;
	chars['o'] = 0b0000000000000000000000011110011111101000010100001011111100111100;
	chars['p'] = 0b0000000000000000000000011110011111101000010100001011111101111100;
	chars['q'] = 0b0000000000000000000000111110011111101000010110011001111000011000;
	chars['r'] = 0b0000000000000000000000000000000000100000010000010011111101111110;
	chars['s'] = 0b0000000000000000000000011001011110101001010100101010011101000100;
	chars['t'] = 0b0000000000000000000000000000010000001000010111111101111110000010;
	chars['u'] = 0b0000000000000000000000111111011111101000000100000011111100111110;
	chars['v'] = 0b0000000000000000000000000011000111101111000111100000111100000110;
	chars['w'] = 0b0000000000000000111110111111001100000011000011000011111100111110;
	chars['x'] = 0b0000000000000000000000100001011101100111100011110011101101000010;
	chars['y'] = 0b0000000000000000000000111111011111100100000010000001111100011110;
	chars['z'] = 0b0000000000000000000000100001010001101001110101101011100101100010;


	chars['A'] = 0b0000000000000001111111111111100000100000010000001011111111111111;
	chars['B'] = 0b0000000000000000000000011110111111111000010100001011111111111111;
	chars['C'] = 0b0000000000000000000000010000011000001000000100000011111110111111;
	chars['D'] = 0b0000000000000000000000011111111111111000000100000011111111111111;
	chars['E'] = 0b0000000000000000000000100000010000001000010100001011111111111111;
	chars['F'] = 0b0000000000000000000000000000000000000000010000001011111111111111;
	chars['G'] = 0b0000000000000000000000011110011111001000100100000011111110111111;
	chars['H'] = 0b0000000000000000000000111111111111110000010000001011111111111111;
	chars['I'] = 0b0000000000000000000000000000000000001000000111111111111111000000;
	chars['J'] = 0b0000000000000000000000011111111111111000000100000011000000100000;
	chars['K'] = 0b0000000000000001100000111000000110010001111000011011111111111111;
	chars['L'] = 0b0000000000000000000000100000010000001000000100000011111111111111;
	chars['M'] = 0b0111111111111110000001000001100001110000011000000111111111111111;
	chars['N'] = 0b0000000000000001111111111111100111000000111000000111111111111111;
	chars['O'] = 0b0000000000000000111111111111110000001000000100000011111110111111;
	chars['P'] = 0b0000000000000000000000000001100001110000100000010011111111111111;
	//Q
	chars['R'] = 0b0000000000000000000000111101111111110000100000010011111111111111;
	chars['S'] = 0b0000000000000000000000011110011111101000110100001111000110100001;
	chars['T'] = 0b0000000000000000000000000000000000000000000000000011111111111111;
	chars['U'] = 0b0000000000000000000000011111111111111000000100000011111110111111;
	chars['V'] = 0b0000000000000000000000000001100111111111100111110000111110000011;
	chars['W'] = 0b0000000000011110111111110000011111101111110110000001111110001111;
	chars['Z'] = 0b0000000000000000000000100000010000011000011100111011111001110000;

	chars['0'] = 0b0000000000000000000000001111101111111100000110000001111110011111;
	chars['1'] = 0b0000000000000000000000000000000000001000000111111111111111000000;
	chars['2'] = 0b0000000000000000000000100000110000111000010100011011111001111000;
	chars['3'] = 0b0000000000000000000000011110111111111000010100001011000000100000;
	chars['4'] = 0b0000000000000000010000111111111111110010000001001100111110011100;
	chars['5'] = 0b0000000000000000000000011111011111111000001100000111000010100001;
	chars['6'] = 0b0000000000000000000000011110011111101000010100001011111110111111;
	chars['7'] = 0b0000000000000000000000000000000000000000011000011111111101111000;
	chars['8'] = 0b0000000000000000000000011110111111111000010100001011111110111101;
	//9

	chars['-'] = 0b0000000000000000000000000010000001000000100000010000001000000100;
	chars['/'] = 0b0000000000000000000000000000000000000000001000111111111101110000;
	chars['('] = 0b0000000000000000000000000000000000000000000100000011111110111111;
	chars[')'] = 0b0000000000000000000000000000000000000000000011111111111111000000;
	return chars;
}

BITMAPINFO Screen::CreateBitmap(const int x, const int y) {
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize			= sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth		= x;
	bitmap.bmiHeader.biHeight		= -y;
	bitmap.bmiHeader.biPlanes		= 1;
	bitmap.bmiHeader.biBitCount		= 32;
	bitmap.bmiHeader.biCompression	= BI_RGB;
	bitmap.bmiHeader.biSizeImage	= x * 4 * y;
	bitmap.bmiHeader.biClrUsed		= 0;
	bitmap.bmiHeader.biClrImportant = 0;
	return bitmap;
}

COLORREF Screen::Color(const BYTE* bitPointer, const int x, const int y, const int width) {
	int index	= y * width + x;
	int blue	= (int) bitPointer[index * 4 + 0];
	int green	= (int) bitPointer[index * 4 + 1];
	int red		= (int) bitPointer[index * 4 + 2];
	return RGB(red, green, blue);
}


bool Screen::Match(const COLORREF& color) {
	for (int i = 0; i < sizeof(colors) / sizeof(*colors); i++) {
		if (colors[i] == color) return true;
	}
	return false;
}

bool Screen::NextCharacter(const BYTE* bitPointer, int& _x, const int _y, const int width) {
	for (int x = _x; x < _x + CHAR_WIDTH + 1; x++) {
		for (int y = _y; y < _y + CHAR_HEIGHT; y++) {
			if (Match(Color(bitPointer, x, y, width))) {
				_x = x;
				return true;
			}
		}
	}
	return false;
}

long long Screen::GetCharacter(const BYTE* bitPointer, int& _x, const int _y, const int width) {
	long long c = 0;
	bool found = false;
	for (int x = _x; x < _x + CHAR_WIDTH + 1; x++) {
		for (int y = _y; y < _y + CHAR_HEIGHT; y++) {
			if (Match(Color(bitPointer, x, y, width))) {
				c |= 1ULL << ((x - _x)*CHAR_HEIGHT + (y - _y));
				found = true;
			}
		}
		if (!found) {
			_x = x;
			return c;
		}
		found = false;
	}
}

char Screen::SolveClash(const BYTE* bitPointer, const int x, const char c, const int width) {
	if (c == 'j' || c == 'i') {
		if (Match(Color(bitPointer, x - 1, 10, width))) return 'j';
		return 'i';
	} else if (c == 'T' || c == 'l') {
		if (Match(Color(bitPointer, x, 0, width))) return 'T';
		return 'l';
	} else if (c == '1' || c == 'I') {
		if (Match(Color(bitPointer, x - 1, 0, width))) return 'I';
		return '1';
	} else {
		return c;
	}
}

char Screen::IdentifyCharacter(const BYTE* bitPointer, const int x, const long long c, const int width) {
	for (std::map<char, long long>::const_iterator i = chars.begin(); i != chars.end(); i++) {
		if ((i->second ^ c) == 0) {
			return SolveClash(bitPointer, x, i->first, width);
		}
	}
}


std::string Screen::ReadHover() {
	// Define the bitmap. We capture a 500x11 area in
	// the top left corner. Characters can actually
	// be 9x15 in size. But to save space and speed
	// the crucial parts of a character can be stored
	// in a 9x7 space (63 bits). The reason to capture
	// 11 pixels in height is to handle the few
	// clashes that occur (think i,j and 1,l...)
	BITMAPINFO bitmap = CreateBitmap(500, 11);

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
	// Top left text is offset by some set pixels.
	BitBlt(deviceContextCopy, 0, 0, 500, 11, deviceContext, 6, 7, SRCCOPY);

	// Immediatly release RuneScape's device context.
	ReleaseDC(client, deviceContext);
	DeleteObject(hBitmap);

	std::string s = "";
	int x = 0, ox = 0;
	// We start scanning for characters 3 pixels
	// below our start point - since this is where
	// the critical information lies. The 3 pixels
	// skipped, aswell as the 1 pixel skipped at
	// the bottom is used to identify clashing
	// characters.
	while (NextCharacter(bitPointer, x, 3, 500)) {
		if (x - ox >= 4) s += ' ';
		long long c = GetCharacter(bitPointer, x, 3, 500);
		s += IdentifyCharacter(bitPointer, x, c, 500);
		ox = x;
	}

	// Delete any context we created.
	DeleteDC(deviceContextCopy);

	return s;
}




COLORREF Screen::ReadPixel(const int x, const int y) {
	// Define the bitmap. We capture a 500x11 area in
	// the top left corner. Characters can actually
	// be 9x15 in size. But to save space and speed
	// the crucial parts of a character can be stored
	// in a 9x7 space (63 bits). The reason to capture
	// 11 pixels in height is to handle the few
	// clashes that occur (think i,j and 1,l...)
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
	// Top left text is offset by some set pixels.
	BitBlt(deviceContextCopy, 0, 0, 1, 1, deviceContext, x, y, SRCCOPY);

	// Immediatly release RuneScape's device context.
	ReleaseDC(client, deviceContext);
	DeleteObject(hBitmap);

	COLORREF c = Color(bitPointer, 0, 0, 1);

	// Delete any context we created.
	DeleteDC(deviceContextCopy);

	return c;
}





Screen::Screen(const HWND _client) {
	client = _client;
}