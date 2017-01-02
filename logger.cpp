#include "logger.h"
#include <string>

spy::Logger::~Logger()
{
  close();
}

bool spy::Logger::open(const char *file_path)
{
  file_.open(file_path);
  if (!file_.good())
  {
    return false;
  }

  log("==================== START LOGGING ====================");

  return true;
}

void spy::Logger::close()
{
  if (file_.is_open())
  {
    log("==================== STOP LOGGING ====================");
    file_.close();
  }
}

void spy::Logger::log(_Printf_format_string_ const char *fmt, ...)
{
  if (file_.is_open())
  {
    va_list ap;

    va_start(ap, fmt);
    vlog(fmt, ap);
    va_end(ap);
  }
}

void spy::Logger::vlog(_Printf_format_string_ const char *fmt, va_list ap)
{
  if (file_.is_open())
  {
    va_list vap;

    va_copy(vap, ap);
    auto size = vsnprintf(nullptr, 0, fmt, vap);
    va_end(vap);

    std::string str;
    str.resize(size + 1);

    va_copy(vap, ap);
    vsnprintf(&str[0], str.size(), fmt, vap);
    va_end(vap);

    file_ << str << std::endl << std::endl;
  }
}
