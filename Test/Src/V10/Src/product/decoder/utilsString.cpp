#include "utilsString.h"
#ifdef __linux__
#include <cwctype>
#endif


using namespace std;


void wmdf::UpperCaseString(basic_string<char>& s)
{
	for (basic_string<char>::iterator p = s.begin(); p != s.end( ); ++p)
	{
		*p = toupper(*p); // toupper is for char
	}
}

void wmdf::UpperCaseString(basic_string<wchar_t>& s)
{
	for (basic_string<wchar_t>::iterator p = s.begin();	p != s.end( ); ++p)
	{
		*p = towupper(*p); // towupper is for wchar_t
	}
}

void wmdf::LowerCaseString(basic_string<char>& s)
{
	for (basic_string<char>::iterator p = s.begin(); p != s.end( ); ++p)
	{
		*p = tolower(*p);
	}
}

void wmdf::LowerCaseString(basic_string<wchar_t>& s)
{
	for (basic_string<wchar_t>::iterator p = s.begin();	p != s.end( ); ++p)
	{
		*p = towlower(*p);
	}
}


