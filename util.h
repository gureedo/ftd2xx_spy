#pragma once

#include <cstdarg>
#include <string>
#include <Windows.h>

#ifndef as_str
#define as_str(X) #X
#endif

namespace spy {
  std::string fmt(_Printf_format_string_ const char *fmt, ...);
  std::string vfmt(_Printf_format_string_ const char *fmt, va_list ap);

  std::string hex_dump(const uint8_t *data, size_t len, const char *prefix);

  std::string GetModuleFileNameA(HMODULE hModule = nullptr);
  std::string FormatMessageA(DWORD dwErrnum);

  std::string GetPrivateProfileStringA(const char *szProfile, const char *szKey, const char *szDefaultValue, const char *szIniFile);

};
