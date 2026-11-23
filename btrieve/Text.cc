#include "Text.h"

#include <cstdlib>
#include <cstring>
#include <vector>

namespace btrieve {

#ifdef _WIN32

std::string toStdString(const wchar_t* str) {
  size_t destSize;
  size_t destBufferSizeInBytes = (wcslen(str) + 1) * sizeof(wchar_t);
  char* destBuffer = reinterpret_cast<char*>(_alloca(destBufferSizeInBytes));

  wcstombs_s(&destSize, destBuffer, destBufferSizeInBytes, str, _TRUNCATE);
  return std::string(destBuffer);
}

std::string toStdString(const std::filesystem::path& dbPath) {
  return toStdString(dbPath.c_str());
}

std::basic_string<wchar_t> toWideString(const std::filesystem::path& dbPath) {
  return dbPath.c_str();
}

std::basic_string<wchar_t> toWideString(const char* str) {
  size_t destSize;
  size_t destSizeInChars = strlen(str) + 1;
  wchar_t* destBuffer =
      reinterpret_cast<wchar_t*>(_alloca(destSizeInChars * sizeof(wchar_t)));

  mbstowcs_s(&destSize, destBuffer, destSizeInChars, str, _TRUNCATE);
  return std::basic_string<wchar_t>(destBuffer);
}

#else

std::string toStdString(const wchar_t *str) {
  // wcstombs's length is in bytes. A multibyte encoding can take up to
  // MB_CUR_MAX bytes per wide char, so size for the worst case plus a
  // terminator. The buffer is zero-filled so a conversion error (which leaves
  // the output partially written) still yields a terminated string.
  std::vector<char> destBuffer(wcslen(str) * MB_CUR_MAX + 1, '\0');

  wcstombs(destBuffer.data(), str, destBuffer.size());
  return std::string(destBuffer.data());
}

std::string toStdString(const std::filesystem::path &dbPath) {
  return dbPath.c_str();
}

std::basic_string<wchar_t> toWideString(const std::filesystem::path &dbPath) {
  return toWideString(dbPath.c_str());
}

std::basic_string<wchar_t> toWideString(const char *str) {
  // mbstowcs's length is in wide chars, not bytes. Each wide char consumes at
  // least one input byte, so strlen(str) + 1 wide chars always suffices. The
  // buffer is zero-filled so a conversion error still yields a terminated
  // string.
  std::vector<wchar_t> destBuffer(strlen(str) + 1, L'\0');

  mbstowcs(destBuffer.data(), str, destBuffer.size());
  return std::basic_string<wchar_t>(destBuffer.data());
}

#endif
}  // namespace btrieve
