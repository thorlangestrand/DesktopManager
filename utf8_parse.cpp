#include "utf8_parse.h"
/**
 * @brief
 * Convert wstring to string
 * @param wstr      input
 * @return          std::string
 */
std::string utf8_encode(const std::wstring& wstr)
{
  // If empty return new string
  if (wstr.empty()) return std::string();
  // Get size of widestring
  int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
  // I don't get it
  std::string strTo(size_needed, 0);
  // Copy data to string
  WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
  return strTo;
}

/**
 * @brief
 * Convert string to wstring
 * @param str     input
 * @return        std::wstring
 */
std::wstring utf8_decode(const std::string& str)
{
  // If empty return new string
  if (str.empty()) return std::wstring();
  // Get size of string
  int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
  // I don't get it
  std::wstring wstrTo(size_needed, 0);
  // Copy data to wstring
  MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
  return wstrTo;
}
