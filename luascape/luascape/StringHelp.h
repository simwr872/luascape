#pragma once
#include <string> 
//https://msdn.microsoft.com/en-us/library/ms235631.aspx
using namespace std;

string w2s(const wchar_t * orig) {
	// Convert the wchar_t string to a char* string. Record   
	// the length of the original string and add 1 to it to  
	// account for the terminating null character.  
	size_t origsize = wcslen(orig) + 1;
	size_t convertedChars = 0;

	// Allocate two bytes in the multibyte output string for every wide  
	// character in the input string (including a wide character  
	// null). Because a multibyte character can be one or two bytes,  
	// you should allot two bytes for each character. Having extra  
	// space for the new string is not an error, but having  
	// insufficient space is a potential security problem.  
	const size_t newsize = origsize * 2;
	// The new string will contain a converted copy of the original  
	// string plus the type of string appended to it.  
	char *nstring = new char[newsize];

	// Put a copy of the converted string into nstring  
	wcstombs_s(&convertedChars, nstring, newsize, orig, _TRUNCATE);
	return nstring;
}

wchar_t * s2w(const char * orig) {
	// newsize describes the length of the   
	// wchar_t string called wcstring in terms of the number   
	// of wide characters, not the number of bytes.  
	size_t newsize = strlen(orig) + 1;

	// The following creates a buffer large enough to contain   
	// the exact number of characters in the original string  
	// in the new format. If you want to add more characters  
	// to the end of the string, increase the value of newsize  
	// to increase the size of the buffer.  
	wchar_t * wcstring = new wchar_t[newsize];

	// Convert char* string to a wchar_t* string.  
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, orig, _TRUNCATE);
	return wcstring;
}

char upper(char c) {
	if (c <= 122 && c >= 97) c -= 32;
	return c;
}