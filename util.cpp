#include "util.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace spy
{
  std::string fmt(_Printf_format_string_ const char *fmt, ...)
  {
    va_list ap;

    va_start(ap, fmt);
    auto str = vfmt(fmt, ap);
    va_end(ap);

    return str;
  }

  std::string vfmt(_Printf_format_string_ const char *fmt, va_list ap)
  {
    va_list vap;

    va_copy(vap, ap);
    auto size = ::vsnprintf(nullptr, 0, fmt, vap);
    va_end(vap);

    std::string str;
    str.resize(size + 1);

    va_copy(vap, ap);
    ::vsnprintf(&str[0], str.size(), fmt, vap);
    va_end(vap);

    return str;
  }

  std::string hex_dump(const uint8_t *data, size_t len, const char *prefix)
  {
    std::stringstream out;

    out << std::hex << std::setfill('0');
    for (size_t pos = 0; pos < len; pos += 16)
    {
      auto nread = min(pos + 16, len) - pos;

      // Show the address
      out << prefix << std::setw(8) << pos;

      // Show the hex codes
      for (size_t i = 0; i < 16; i++)
      {
        if (i % 8 == 0) out << ' ';
        if (i < nread)
          out << ' ' << std::setw(2) << (unsigned)data[pos + i];
        else
          out << "   ";
      }

      // Show printable characters
      out << "  ";
      for (size_t i = 0; i < nread; i++)
      {
        if (data[pos + i] < 32)
          out << '.';
        else
          out << data[pos + i];
      }

      out << "\n";
    }

    out.get();
    return out.str();
  }

  std::string GetModuleFileNameA(HMODULE hModule)
  {
    std::string strFileName;
    DWORD copied = 0;
    do {
      strFileName.resize(strFileName.size() + MAX_PATH);
      copied = ::GetModuleFileNameA(0, &strFileName[0], (DWORD)strFileName.size());
    } while (copied >= strFileName.size());

    strFileName.resize(copied);
    return strFileName;
  }



  std::string FormatMessageA(DWORD dwErrnum)
  {
    char *szBuffer = nullptr;

    ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr, dwErrnum, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPSTR)&szBuffer, 0, nullptr);

    std::string strError;
    if (szBuffer) {
      strError = szBuffer;
      strError.resize(strError.size() - 1); // remove trailing \n
      ::LocalFree(szBuffer);
    }
    else
    {
      strError = "<FAILED TO RETRIEVE ERROR STRING>";
    }

    return strError;
  }

  std::string GetPrivateProfileStringA(const char *szProfile, const char *szKey, const char *szDefaultValue, const char *szIniFile)
  {
    std::string buf;
    DWORD ret = 0;

    while (((DWORD)buf.size() - ret) < 3) {
      buf.resize(buf.size() + MAX_PATH);
      ret = ::GetPrivateProfileStringA(szProfile, szKey, szDefaultValue, &buf[0], (DWORD)buf.size(), szIniFile);
    }
    buf.resize(ret);

    return buf;
  }
}
