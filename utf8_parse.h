#ifndef UTF8_PARSE_H
#define UTF8_PARSE_H

#include <string>
#include <stringapiset.h>

std::string utf8_encode(const std::wstring& wstr);
std::wstring utf8_decode(const std::string& str);

#endif // UTF8_PARSE_H
