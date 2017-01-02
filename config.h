#pragma once

#include <map>
#include <string>

namespace spy
{
  struct Config
  {
    void Load(const char *file_path);
    const std::string &RewriteDeviceDescription(const char *descr);

    std::string log_file_path;
    std::string real_dll_path;
    bool ignore_missing_functions;
    bool enable_file_logging;
    bool enable_console_logging;
    bool safe_mode;

  private:
    std::string config_file_path_;
    std::map<std::string, std::string> rewrite_descr_;
  };
}
