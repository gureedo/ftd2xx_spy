#pragma once

#include <cstdarg>
#include <fstream>
#include <memory>

namespace spy
{
  class Logger
  {
  public:
    Logger() = default;
    Logger(Logger &&other) = delete;
    Logger(const Logger &other) = delete;
    Logger &operator=(Logger &&other) = delete;
    Logger &operator=(const Logger &other) = delete;
    ~Logger();

    bool open(const char *file_path);
    void close();

    void log(_Printf_format_string_ const char *fmt, ...);
    void vlog(_Printf_format_string_ const char *fmt, va_list ap);

  private:
    std::ofstream file_;
  };
}
