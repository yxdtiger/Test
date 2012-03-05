#ifndef _UTILSSTRING_H
#define _UTILSSTRING_H

#include <string>


namespace wmdf
{

  void UpperCaseString(std::basic_string<char>& s);
  void UpperCaseString(std::basic_string<wchar_t>& s);
  void LowerCaseString(std::basic_string<char>& s);
  void LowerCaseString(std::basic_string<wchar_t>& s);

}

#endif  // _UTILSSTRING