#include "config.h"
#include "Windows.h"

#include "util.h"

namespace spy
{
  constexpr char kAppName[] = "ftd2xx_spy";

  void Config::Load(const char *file_path)
  {
    config_file_path_ = file_path;
    log_file_path = GetPrivateProfileStringA(kAppName, "LogFilePath", "ftd2xx_spy.log", file_path);
    real_dll_path = GetPrivateProfileStringA(kAppName, "RealFtd2xxDllPath", "ftd2xx_real.dll", file_path);
    ignore_missing_functions = ::GetPrivateProfileIntA(kAppName, "IgnoreMissingFunctions", 1, file_path) != 0;
    enable_file_logging = ::GetPrivateProfileIntA(kAppName, "EnableFileLogging", 1, file_path) != 0;
    enable_console_logging = ::GetPrivateProfileIntA(kAppName, "EnableConsoleLogging", 0, file_path) != 0;
    safe_mode = ::GetPrivateProfileIntA(kAppName, "SafeMode", 1, file_path) != 0;
  }

  const std::string &Config::RewriteDeviceDescription(const char *descr)
  {
    auto iter = rewrite_descr_.find(descr);
    if (iter != rewrite_descr_.end())
      return iter->second;

    std::string rewrite = GetPrivateProfileStringA("RewriteDeviceDescription", descr, descr, config_file_path_.c_str()).c_str();
    rewrite_descr_[descr] = rewrite;

    return rewrite_descr_[descr];
  }
}

