#pragma once
#include <string>
class StringHelp {
public:
	static const char* w2s(const wchar_t * orig);
	static wchar_t * s2w(const char * orig);
	static char upper(char c);
private:
	StringHelp() {};
};