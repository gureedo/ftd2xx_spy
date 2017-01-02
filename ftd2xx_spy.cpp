#include "ftd2xx_spy.h"
#include <cassert>
#include <cinttypes>
#include <map>

#include "util.h"

#define FTD2XX_SPY_LOAD_REAL_FN(fn_name) \
  if ((fns_.##fn_name = reinterpret_cast<t_##fn_name>(::GetProcAddress(dll_handle_, #fn_name))) == nullptr) \
  { \
    logger.log("Missing function " #fn_name); \
    if (!cfg_.ignore_missing_functions) \
      return std::make_pair(false, "Missing function " #fn_name); \
  }

#define FTD2XX_SPY_DICT_ITEM(X) { X, #X }

#define FTD2XX_SPY_TEST_AND_APPEND(V, F, S) \
  if ((V) & (F)) S += #F ", "

namespace spy
{
  Ftd2xxSpy::~Ftd2xxSpy()
  {
    Close();
  }

  std::pair<bool, std::string> Ftd2xxSpy::Init(Config &config)
  {
    cfg_ = config;

    if (cfg_.enable_file_logging)
    {
      if (!logger.open(cfg_.log_file_path.c_str()))
        return std::make_pair(false, "Failed to open log file for writing!");
    }

    dll_handle_ = ::LoadLibraryA(cfg_.real_dll_path.c_str());
    if (!dll_handle_)
    {
      return std::make_pair(false, FormatMessageA(GetLastError()));
    }

    FTD2XX_SPY_LOAD_REAL_FN(FT_GetLibraryVersion);

    DWORD dwRealVersion = 0;
    if (FT_SUCCESS(fns_.FT_GetLibraryVersion(&dwRealVersion)))
      logger.log("Real ftd2xx.dll version: %" PRIX32, dwRealVersion);
    else
      logger.log("Failed to get real ftd2xx.dll version.");

    fns_.FT_Initialize = reinterpret_cast<t_FT_Initialize>(::GetProcAddress(dll_handle_, "_FT_Initialise@0"));
    if (!fns_.FT_Initialize)
    {
      logger.log("Missing function _FT_Initialise@0");
      if (!cfg_.ignore_missing_functions)
        return std::make_pair(false, "Missing function _FT_Initialise@0");
    }

    fns_.FT_Finalise = reinterpret_cast<t_FT_Finalise>(::GetProcAddress(dll_handle_, "_FT_Finalise@0"));
    if (!fns_.FT_Finalise)
    {
      logger.log("Missing function _FT_Finalise@0");
      if (!cfg_.ignore_missing_functions)
        return std::make_pair(false, "Missing function _FT_Finalise@0");
    }

    FTD2XX_SPY_LOAD_REAL_FN(FT_Open);
    FTD2XX_SPY_LOAD_REAL_FN(FT_OpenEx);
    FTD2XX_SPY_LOAD_REAL_FN(FT_ListDevices);
    FTD2XX_SPY_LOAD_REAL_FN(FT_Close);
    FTD2XX_SPY_LOAD_REAL_FN(FT_Read);
    FTD2XX_SPY_LOAD_REAL_FN(FT_Write);
    FTD2XX_SPY_LOAD_REAL_FN(FT_IoCtl);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetBaudRate);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetDivisor);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetDataCharacteristics);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetFlowControl);
    FTD2XX_SPY_LOAD_REAL_FN(FT_ResetDevice);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetDtr);
    FTD2XX_SPY_LOAD_REAL_FN(FT_ClrDtr);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetRts);
    FTD2XX_SPY_LOAD_REAL_FN(FT_ClrRts);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetModemStatus);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetChars);
    FTD2XX_SPY_LOAD_REAL_FN(FT_Purge);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetTimeouts);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetQueueStatus);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetEventNotification);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetStatus);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetBreakOn);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetBreakOff);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetWaitMask);
    FTD2XX_SPY_LOAD_REAL_FN(FT_WaitOnMask);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetEventStatus);
    FTD2XX_SPY_LOAD_REAL_FN(FT_ReadEE);
    FTD2XX_SPY_LOAD_REAL_FN(FT_WriteEE);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EraseEE);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EE_Program);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EE_ProgramEx);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EE_Read);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EE_ReadEx);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EE_UASize);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EE_UAWrite);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EE_UARead);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EEPROM_Read);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EEPROM_Program);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetLatencyTimer);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetLatencyTimer);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetBitMode);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetBitMode);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetUSBParameters);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetDeadmanTimeout);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetDeviceInfo);
    FTD2XX_SPY_LOAD_REAL_FN(FT_StopInTask);
    FTD2XX_SPY_LOAD_REAL_FN(FT_RestartInTask);
    FTD2XX_SPY_LOAD_REAL_FN(FT_SetResetPipeRetryCount);
    FTD2XX_SPY_LOAD_REAL_FN(FT_ResetPort);
    FTD2XX_SPY_LOAD_REAL_FN(FT_CyclePort);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_CreateFile);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_CloseHandle);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_ReadFile);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_WriteFile);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_GetLastError);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_GetOverlappedResult);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_CancelIo);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_ClearCommBreak);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_ClearCommError);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_EscapeCommFunction);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_GetCommModemStatus);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_GetCommState);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_GetCommTimeouts);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_PurgeComm);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_SetCommBreak);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_SetCommMask);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_GetCommMask);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_SetCommState);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_SetCommTimeouts);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_SetupComm);
    FTD2XX_SPY_LOAD_REAL_FN(FT_W32_WaitCommEvent);
    FTD2XX_SPY_LOAD_REAL_FN(FT_CreateDeviceInfoList);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetDeviceInfoList);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetDeviceInfoDetail);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetDriverVersion);
    FTD2XX_SPY_LOAD_REAL_FN(FT_Rescan);
    FTD2XX_SPY_LOAD_REAL_FN(FT_Reload);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetComPortNumber);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EE_ReadConfig);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EE_WriteConfig);
    FTD2XX_SPY_LOAD_REAL_FN(FT_EE_ReadECC);
    FTD2XX_SPY_LOAD_REAL_FN(FT_GetQueueStatusEx);
    FTD2XX_SPY_LOAD_REAL_FN(FT_ComPortIdle);
    FTD2XX_SPY_LOAD_REAL_FN(FT_ComPortCancelIdle);
    FTD2XX_SPY_LOAD_REAL_FN(FT_VendorCmdGet);
    FTD2XX_SPY_LOAD_REAL_FN(FT_VendorCmdSet);
    FTD2XX_SPY_LOAD_REAL_FN(FT_VendorCmdGetEx);
    FTD2XX_SPY_LOAD_REAL_FN(FT_VendorCmdSetEx);

    return std::make_pair(true, "");
  }

  void Ftd2xxSpy::Close()
  {
    if (dll_handle_)
    {
      FreeLibrary(dll_handle_);
      dll_handle_ = nullptr;
    }
  }

  void Ftd2xxSpy::StoreHandle(FT_HANDLE handle, std::string &descr)
  {
    auto item = handles_.find(handle);
    if (item != handles_.end())
    {
      logger.log("WARNING: Rewriting opened handle description \"%s\" => \"%s\"", item->second.c_str(), descr.c_str());
    }

    handles_[handle] = descr;
  }

  void Ftd2xxSpy::RemoveHandle(FT_HANDLE handle)
  {
    handles_.erase(handle);
  }

  std::string Ftd2xxSpy::ResolveHandle(FT_HANDLE handle)
  {
    auto item = handles_.find(handle);
    if (item == handles_.end())
    {
      return fmt("UNKNOWN(0x%p)", handle);
    }

    return item->second;
  }

  std::string Ftd2xxSpy::FT_STATUS_str(FT_STATUS status)
  {
    static std::map<FT_STATUS, std::string> dict{
      FTD2XX_SPY_DICT_ITEM(FT_OK),
      FTD2XX_SPY_DICT_ITEM(FT_INVALID_HANDLE),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_NOT_FOUND),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_NOT_OPENED),
      FTD2XX_SPY_DICT_ITEM(FT_IO_ERROR),
      FTD2XX_SPY_DICT_ITEM(FT_INSUFFICIENT_RESOURCES),
      FTD2XX_SPY_DICT_ITEM(FT_INVALID_PARAMETER),
      FTD2XX_SPY_DICT_ITEM(FT_INVALID_BAUD_RATE),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_NOT_OPENED_FOR_ERASE),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_NOT_OPENED_FOR_WRITE),
      FTD2XX_SPY_DICT_ITEM(FT_FAILED_TO_WRITE_DEVICE),
      FTD2XX_SPY_DICT_ITEM(FT_EEPROM_READ_FAILED),
      FTD2XX_SPY_DICT_ITEM(FT_EEPROM_WRITE_FAILED),
      FTD2XX_SPY_DICT_ITEM(FT_EEPROM_ERASE_FAILED),
      FTD2XX_SPY_DICT_ITEM(FT_EEPROM_NOT_PRESENT),
      FTD2XX_SPY_DICT_ITEM(FT_EEPROM_NOT_PROGRAMMED),
      FTD2XX_SPY_DICT_ITEM(FT_INVALID_ARGS),
      FTD2XX_SPY_DICT_ITEM(FT_NOT_SUPPORTED),
      FTD2XX_SPY_DICT_ITEM(FT_OTHER_ERROR),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_LIST_NOT_READY),
    };

    auto item = dict.find(status);
    if (item == dict.end())
    {
      return fmt("UNKNOWN(%ul)", status);
    }

    return item->second;
  }

  std::string Ftd2xxSpy::FT_ListDevices_Flags_str(DWORD flags)
  {
    std::string str;

    FTD2XX_SPY_TEST_AND_APPEND(flags, FT_LIST_NUMBER_ONLY, str);
    FTD2XX_SPY_TEST_AND_APPEND(flags, FT_LIST_BY_INDEX, str);
    FTD2XX_SPY_TEST_AND_APPEND(flags, FT_LIST_ALL, str);

    str += FT_OpenEx_Flags_str(flags);

    return str;
  }

  std::string Ftd2xxSpy::FT_OpenEx_Flags_str(DWORD flags)
  {
    std::string str;

    FTD2XX_SPY_TEST_AND_APPEND(flags, FT_OPEN_BY_SERIAL_NUMBER, str);
    FTD2XX_SPY_TEST_AND_APPEND(flags, FT_OPEN_BY_DESCRIPTION, str);
    FTD2XX_SPY_TEST_AND_APPEND(flags, FT_OPEN_BY_LOCATION, str);

    return str;
  }

  std::string Ftd2xxSpy::FT_PARITY_str(UCHAR parity)
  {
    static std::map<UCHAR, std::string> dict{
      FTD2XX_SPY_DICT_ITEM(FT_PARITY_NONE),
      FTD2XX_SPY_DICT_ITEM(FT_PARITY_ODD),
      FTD2XX_SPY_DICT_ITEM(FT_PARITY_EVEN),
      FTD2XX_SPY_DICT_ITEM(FT_PARITY_MARK),
      FTD2XX_SPY_DICT_ITEM(FT_PARITY_SPACE),
    };

    auto item = dict.find(parity);
    if (item != dict.end())
      return item->second;

    return fmt("UNKNOWN(0x%" PRIX8 ")", parity);
  }

  std::string Ftd2xxSpy::FT_BITS_str(UCHAR bits)
  {
    if (bits == FT_BITS_8)
      return "FT_BITS_8";
    else if (bits == FT_BITS_7)
      return "FT_BITS_7";
    else
      return fmt("UNKNOWN(0x%" PRIX8 ")", bits);
  }

  std::string Ftd2xxSpy::FT_STOP_BITS_str(UCHAR bits)
  {
    if (bits == FT_STOP_BITS_1)
      return "FT_STOP_BITS_1";
    else if (bits == FT_STOP_BITS_2)
      return "FT_STOP_BITS_2";
    else
      return fmt("UNKNOWN(0x%" PRIX8 ")", bits);
  }

  std::string Ftd2xxSpy::FT_FLOW_str(USHORT flow)
  {
    if (flow == FT_FLOW_NONE)
      return "FT_FLOW_NONE";
    else if (flow == FT_FLOW_RTS_CTS)
      return "FT_FLOW_RTS_CTS";
    else   if (flow == FT_FLOW_DTR_DSR)
      return "FT_FLOW_DTR_DSR";
    else if (flow == FT_FLOW_XON_XOFF)
      return "FT_FLOW_XON_XOFF";
    else
      return fmt("UNKNOWN(0x%" PRIX16 ")", flow);
  }

  std::string Ftd2xxSpy::FT_PURGE_str(ULONG mask)
  {
    std::string str;

    FTD2XX_SPY_TEST_AND_APPEND(mask, FT_PURGE_RX, str);
    FTD2XX_SPY_TEST_AND_APPEND(mask, FT_PURGE_TX, str);

    return str;
  }

  std::string Ftd2xxSpy::FT_EVENT_str(DWORD mask)
  {
    std::string str;

    FTD2XX_SPY_TEST_AND_APPEND(mask, FT_EVENT_RXCHAR, str);
    FTD2XX_SPY_TEST_AND_APPEND(mask, FT_EVENT_MODEM_STATUS, str);
    FTD2XX_SPY_TEST_AND_APPEND(mask, FT_EVENT_LINE_STATUS, str);

    return str;
  }

  std::string Ftd2xxSpy::FT_DEVICE_str(FT_DEVICE device)
  {
    static std::map<FT_DEVICE, std::string> dict{
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_BM),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_AM),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_100AX),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_UNKNOWN),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_2232C),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_232R),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_2232H),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_4232H),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_232H),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_X_SERIES),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_4222H_0),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_4222H_1_2),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_4222H_3),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_4222_PROG),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_900),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_930),
      FTD2XX_SPY_DICT_ITEM(FT_DEVICE_UMFTPD3A),
    };

    auto item = dict.find(device);
    if (item == dict.end())
    {
      return fmt("UNKNOWN(%ul)", device);
    }

    return item->second;
  }

  std::string Ftd2xxSpy::FT_W32_CreateFile_Access_str(DWORD dwAccess)
  {
    std::string str;

    FTD2XX_SPY_TEST_AND_APPEND(dwAccess, GENERIC_READ, str);
    FTD2XX_SPY_TEST_AND_APPEND(dwAccess, GENERIC_WRITE, str);
    FTD2XX_SPY_TEST_AND_APPEND(dwAccess, GENERIC_EXECUTE, str);
    FTD2XX_SPY_TEST_AND_APPEND(dwAccess, GENERIC_ALL, str);

    return str;
  }

  std::string Ftd2xxSpy::FT_W32_CreateFile_Create_str(DWORD dwCreate)
  {
    std::string str;

    FTD2XX_SPY_TEST_AND_APPEND(dwCreate, CREATE_NEW, str);
    FTD2XX_SPY_TEST_AND_APPEND(dwCreate, CREATE_ALWAYS, str);
    FTD2XX_SPY_TEST_AND_APPEND(dwCreate, OPEN_EXISTING, str);
    FTD2XX_SPY_TEST_AND_APPEND(dwCreate, OPEN_ALWAYS, str);
    FTD2XX_SPY_TEST_AND_APPEND(dwCreate, TRUNCATE_EXISTING, str);

    return str;
  }

  std::string Ftd2xxSpy::FT_W32_CreateFile_AttrsAndFlags_str(DWORD dwAttrsAndFlags)
  {
    std::string str;

    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_READONLY, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_HIDDEN, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_SYSTEM, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_DIRECTORY, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_ARCHIVE, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_DEVICE, str);
    FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_NORMAL, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_TEMPORARY, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_SPARSE_FILE, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_REPARSE_POINT, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_COMPRESSED, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_OFFLINE, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_NOT_CONTENT_INDEXED, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_ENCRYPTED, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_INTEGRITY_STREAM, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_VIRTUAL, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_NO_SCRUB_DATA, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_ATTRIBUTE_EA, str);

    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_WRITE_THROUGH, str);
    FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_OVERLAPPED, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_NO_BUFFERING, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_RANDOM_ACCESS, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_SEQUENTIAL_SCAN, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_DELETE_ON_CLOSE, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_BACKUP_SEMANTICS, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_POSIX_SEMANTICS, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_SESSION_AWARE, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_OPEN_REPARSE_POINT, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_OPEN_NO_RECALL, str);
    //FTD2XX_SPY_TEST_AND_APPEND(dwAttrsAndFlags, FILE_FLAG_FIRST_PIPE_INSTANCE, str);

    str += FT_OpenEx_Flags_str(dwAttrsAndFlags);

    return str;
  }

  std::string Ftd2xxSpy::FT_DEVICE_LIST_INFO_NODE_str(const FT_DEVICE_LIST_INFO_NODE *node)
  {
    return fmt(
      "handle: %s, serial: \"%s\", descr: \"%s\", locId: %d, flags: %X, type: %X, id: %X",
      ResolveHandle(node->ftHandle).c_str(),
      node->SerialNumber,
      node->Description,
      node->LocId,
      node->Flags,
      node->Type,
      node->ID
    );
  }

  /*
   * FTD2XX FUNCTIONS
   */

  void Ftd2xxSpy::FT_Initialize()
  {
    fns_.FT_Initialize();
    logger.log("Call %s()", __func__);
  }

  void Ftd2xxSpy::FT_Finalise()
  {
    fns_.FT_Finalise();
    logger.log("Call %s()", __func__);
  }

  FT_STATUS Ftd2xxSpy::FT_Open(int deviceNumber, FT_HANDLE *pHandle)
  {
    auto ret = fns_.FT_Open(deviceNumber, pHandle);

    if (FT_SUCCESS(ret))
    {
      auto descr = fmt("deviceId:%d", deviceNumber);
      StoreHandle(*pHandle, descr);
    }

    logger.log(
      "Call %s() => %s\n"
      "\tdeviceNumber = %d\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      deviceNumber,
      ResolveHandle(*pHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_OpenEx(PVOID pArg1, DWORD Flags, FT_HANDLE *pHandle)
  {
    if (Flags & FT_OPEN_BY_DESCRIPTION)
      pArg1 = (PVOID)cfg_.RewriteDeviceDescription((char *)pArg1).c_str();

    auto ret = fns_.FT_OpenEx(pArg1, Flags, pHandle);

    std::string pArg1_descr;
    std::string descr;

    if (Flags & (FT_OPEN_BY_SERIAL_NUMBER | FT_OPEN_BY_DESCRIPTION))
    {
      pArg1_descr = fmt("\"%s\"", (char *)pArg1);
      if (Flags & FT_OPEN_BY_SERIAL_NUMBER)
        descr = fmt("serial:%s", pArg1_descr.c_str());
      else
        descr = fmt("descr:%s", pArg1_descr.c_str());
    }
    else if (Flags & FT_OPEN_BY_LOCATION)
    {
      pArg1_descr = fmt("%d", (DWORD)pArg1);
      descr = fmt("locId:%s", pArg1_descr.c_str());
    }
    else
    {
      pArg1_descr = fmt("0x%p", pArg1);
      descr = fmt("unknown:0x%p", *pHandle);
    }

    if (FT_SUCCESS(ret))
      StoreHandle(*pHandle, descr);

    logger.log(
      "Call %s() => %s\n"
      "\tpArg1 = %s\n"
      "\tFlags = %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      pArg1_descr.c_str(),
      FT_OpenEx_Flags_str(Flags).c_str(),
      ResolveHandle(*pHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_ListDevices(PVOID pArg1, PVOID pArg2, DWORD Flags)
  {
    auto ret = fns_.FT_ListDevices(pArg1, pArg2, Flags);

    std::string pArg1_descr;
    std::string pArg2_descr;

    if (Flags & FT_LIST_NUMBER_ONLY)
    {
      pArg1_descr = fmt("%d (number of devices)", *static_cast<PDWORD>(pArg1));
      pArg2_descr = fmt("0x%p (unknown)", pArg2);
    }
    else if (Flags & FT_LIST_BY_INDEX)
    {
      pArg1_descr = fmt("%d (device index)", reinterpret_cast<DWORD>(pArg1));
      if (Flags & (FT_OPEN_BY_SERIAL_NUMBER | FT_OPEN_BY_DESCRIPTION))
      {
        pArg2_descr = fmt("\"%s\"", static_cast<char *>(pArg2));
      }
      else if (Flags & FT_OPEN_BY_LOCATION)
      {
        pArg2_descr = fmt("%d", *static_cast<PDWORD>(pArg2));
      }
      else
      {
        pArg2_descr = fmt("0x%p", pArg2);
      }
    }
    else if (Flags & FT_LIST_ALL)
    {
      if (Flags & (FT_OPEN_BY_SERIAL_NUMBER | FT_OPEN_BY_DESCRIPTION))
      {
        auto pszData = static_cast<char **>(pArg1);
        for (size_t i = 0; pszData[i]; i++)
        {
          pArg1_descr = fmt("\"%s\", ", pszData[i]);
        }
      }
      else if (Flags & FT_OPEN_BY_LOCATION)
      {
        auto pdwData = static_cast<DWORD *>(pArg1);
        for (size_t i = 0; pdwData[i]; i++)
        {
          pArg1_descr = fmt("%d, ", pdwData[i]);
        }
      }
      else
      {
        pArg1_descr = fmt("0x%p", pArg1);
      }
      pArg2_descr = fmt("%d (number of devices)", *static_cast<PDWORD>(pArg2));
    }
    else
    {
      pArg1_descr = fmt("0x%p", pArg1);
      pArg2_descr = fmt("0x%p", pArg2);
    }

    logger.log(
      "Call %s() => %s\n"
      "\tpArg1 = %s\n"
      "\tpArg2 = %s\n"
      "\tFlags = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      pArg1_descr.c_str(),
      pArg2_descr.c_str(),
      FT_ListDevices_Flags_str(Flags).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_Close(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_Close(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    if (FT_SUCCESS(ret))
      RemoveHandle(ftHandle);

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_Read(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToRead, LPDWORD lpBytesReturned)
  {
    auto ret = fns_.FT_Read(ftHandle, lpBuffer, dwBytesToRead, lpBytesReturned);;

    // TODO: log data
    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tdata[%d] =\n"
      "%s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *lpBytesReturned,
      hex_dump((uint8_t*)lpBuffer, *lpBytesReturned, "\t\t").c_str()
    );

    return ret;

  }

  FT_STATUS Ftd2xxSpy::FT_Write(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToWrite, LPDWORD lpBytesWritten)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_Write(ftHandle, lpBuffer, dwBytesToWrite, lpBytesWritten);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";
    if (cfg_.safe_mode)
      *lpBytesWritten = dwBytesToWrite;

    // TODO: log data
    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\tbytesWritten = %d\n"
      "\tdata[%d] =\n"
      "%s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      *lpBytesWritten,
      dwBytesToWrite,
      hex_dump((uint8_t*)lpBuffer, dwBytesToWrite, "\t\t").c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_IoCtl(FT_HANDLE ftHandle, DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_IoCtl(ftHandle, dwIoControlCode, lpInBuf, nInBufSize, lpOutBuf, nOutBufSize, lpBytesReturned, lpOverlapped);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";
    // TODO: update lpBytesReturned in safe mode

    // TODO: log data
    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\tdwIoControlCode = %d\n"
      "\tlpInBuf = 0x%p\n"
      "\tnInBufSize = %d\n"
      "\tlpOutBuf = 0x%p\n"
      "\tnOutBufSize = %d\n"
      "\tlpBytesReturned = %d\n"
      "\tlpOverlapped = 0x%p",
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      dwIoControlCode,
      lpInBuf,
      nInBufSize,
      lpOutBuf,
      nOutBufSize,
      *lpBytesReturned,
      lpOverlapped
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetBaudRate(FT_HANDLE ftHandle, ULONG BaudRate)
  {
    auto ret = fns_.FT_SetBaudRate(ftHandle, BaudRate);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tBaudRate = %ul",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      BaudRate
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetDivisor(FT_HANDLE ftHandle, USHORT Divisor)
  {
    auto ret = fns_.FT_SetDivisor(ftHandle, Divisor);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tDivisor = %" PRIu16,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      Divisor
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetDataCharacteristics(FT_HANDLE ftHandle, UCHAR WordLength, UCHAR StopBits, UCHAR Parity)
  {
    auto ret = fns_.FT_SetDataCharacteristics(ftHandle, WordLength, StopBits, Parity);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tWordLength = %s\n"
      "\tStopBits = %s\n"
      "\tParity = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      FT_BITS_str(WordLength).c_str(),
      FT_STOP_BITS_str(StopBits).c_str(),
      FT_PARITY_str(Parity).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetFlowControl(FT_HANDLE ftHandle, USHORT FlowControl, UCHAR XonChar, UCHAR XoffChar)
  {
    auto ret = fns_.FT_SetFlowControl(ftHandle, FlowControl, XonChar, XoffChar);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tFlowControl = %s\n"
      "\tXonChar = 0x%" PRIX8 "\n"
      "\tXoffChar = 0x%" PRIX8,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      FT_FLOW_str(FlowControl).c_str(),
      XonChar,
      XoffChar
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_ResetDevice(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_ResetDevice(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetDtr(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_SetDtr(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_ClrDtr(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_ClrDtr(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetRts(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_SetRts(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_ClrRts(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_ClrRts(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetModemStatus(FT_HANDLE ftHandle, ULONG *pModemStatus)
  {
    auto ret = fns_.FT_GetModemStatus(ftHandle, pModemStatus);

    // TODO: decode modem status
    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tModemStatus = 0x%" PRIX32,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *pModemStatus
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetChars(FT_HANDLE ftHandle, UCHAR EventChar, UCHAR EventCharEnabled, UCHAR ErrorChar, UCHAR ErrorCharEnabled)
  {
    auto ret = fns_.FT_SetChars(ftHandle, EventChar, EventCharEnabled, ErrorChar, ErrorCharEnabled);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tEventChar = %" PRIX8 "\n"
      "\tEventCharEnabled = %" PRIX8 "\n"
      "\tErrorChar = %" PRIX8 "\n"
      "\tErrorCharEnabled = %" PRIX8,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      EventChar,
      EventCharEnabled,
      ErrorChar,
      ErrorCharEnabled
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_Purge(FT_HANDLE ftHandle, ULONG Mask)
  {
    auto ret = fns_.FT_Purge(ftHandle, Mask);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tMask = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      FT_PURGE_str(Mask).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetTimeouts(FT_HANDLE ftHandle, ULONG ReadTimeout, ULONG WriteTimeout)
  {
    auto ret = fns_.FT_SetTimeouts(ftHandle, ReadTimeout, WriteTimeout);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tReadTimeout = %d\n"
      "\tWriteTimeout = %d",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      ReadTimeout,
      WriteTimeout
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetQueueStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes)
  {
    auto ret = fns_.FT_GetQueueStatus(ftHandle, dwRxBytes);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tdwRxBytes = %d",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *dwRxBytes
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetEventNotification(FT_HANDLE ftHandle, DWORD Mask, PVOID Param)
  {
    auto ret = fns_.FT_SetEventNotification(ftHandle, Mask, Param);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tMask = %s\n"
      "\tParam = 0x%p",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      FT_EVENT_str(Mask).c_str(),
      Param
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes, DWORD *dwTxBytes, DWORD *dwEventDWord)
  {
    auto ret = fns_.FT_GetStatus(ftHandle, dwRxBytes, dwTxBytes, dwEventDWord);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tdwRxBytes = %d\n"
      "\tdwTxBytes = %d\n"
      "\tdwEventDWord = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *dwRxBytes,
      *dwTxBytes,
      FT_EVENT_str(*dwEventDWord).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetBreakOn(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_SetBreakOn(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetBreakOff(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_SetBreakOff(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetWaitMask(FT_HANDLE ftHandle, DWORD Mask)
  {
    auto ret = fns_.FT_SetWaitMask(ftHandle, Mask);

    // TODO: decode mask
    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tMask = 0x%" PRIX32,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      Mask
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_WaitOnMask(FT_HANDLE ftHandle, DWORD *Mask)
  {
    auto ret = fns_.FT_WaitOnMask(ftHandle, Mask);

    // TODO: decode mask
    logger.log(
      "Call %s() => %s"
      "\thandle = %s\n"
      "\tMask = 0x%" PRIX32,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *Mask
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetEventStatus(FT_HANDLE ftHandle, DWORD *dwEventDWord)
  {
    auto ret = fns_.FT_GetEventStatus(ftHandle, dwEventDWord);

    // TODO: decode events
    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tdwEventDWord = 0x%" PRIX32,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *dwEventDWord
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_ReadEE(FT_HANDLE ftHandle, DWORD dwWordOffset, LPWORD lpwValue)
  {
    auto ret = fns_.FT_ReadEE(ftHandle, dwWordOffset, lpwValue);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\toffset = 0x%X\n"
      "\tvalue = 0x%04" PRIX16,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      dwWordOffset,
      *lpwValue
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_WriteEE(FT_HANDLE ftHandle, DWORD dwWordOffset, WORD wValue)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_WriteEE(ftHandle, dwWordOffset, wValue);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\toffset = 0x%X\n"
      "\tvalue = 0x%04" PRIX16,
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      dwWordOffset,
      wValue
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EraseEE(FT_HANDLE ftHandle)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_EraseEE(ftHandle);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EE_Program(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_EE_Program(ftHandle, pData);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    // TODO: decode program
    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\tpData = 0x%p",
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      pData
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EE_ProgramEx(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_EE_ProgramEx(ftHandle, pData, Manufacturer, ManufacturerId, Description, SerialNumber);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    // TODO: decode program
    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\tpData = 0x%p\n"
      "\tManufacturer = %s\n"
      "\tManufacturerId = %s\n"
      "\tDescription = %s\n"
      "\tSerialNumber = %s\n",
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      pData,
      Manufacturer,
      ManufacturerId, Description,
      SerialNumber
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EE_Read(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData)
  {
    auto ret = fns_.FT_EE_Read(ftHandle, pData);

    // TODO: decode program
    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tpData = 0x%p",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      pData
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EE_ReadEx(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber)
  {
    auto ret = fns_.FT_EE_ReadEx(ftHandle, pData, Manufacturer, ManufacturerId, Description, SerialNumber);

    // TODO: decode program
    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tpData = 0x%p\n"
      "\tManufacturer = %s\n"
      "\tManufacturerId = %s\n"
      "\tDescription = %s\n"
      "\tSerialNumber = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      pData,
      Manufacturer,
      ManufacturerId,
      Description,
      SerialNumber
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EE_UASize(FT_HANDLE ftHandle, LPDWORD lpdwSize)
  {
    auto ret = fns_.FT_EE_UASize(ftHandle, lpdwSize);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tdwSize = %d",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *lpdwSize
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EE_UAWrite(FT_HANDLE ftHandle, PUCHAR pucData, DWORD dwDataLen)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_EE_UAWrite(ftHandle, pucData, dwDataLen);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    // TODO: log data
    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\tpucData => 0x%p\n"
      "\tdataLen = %d",
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      pucData,
      dwDataLen
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EE_UARead(FT_HANDLE ftHandle, PUCHAR pucData, DWORD dwDataLen, LPDWORD lpdwBytesRead)
  {
    auto ret = fns_.FT_EE_UARead(ftHandle, pucData, dwDataLen, lpdwBytesRead);

    // TODO: log data
    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tpucData = 0x%p\n"
      "\tdwDataLen = %d\n"
      "\tdwBytesRead = %d",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      pucData,
      dwDataLen,
      *lpdwBytesRead
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EEPROM_Read(FT_HANDLE ftHandle, void *eepromData, DWORD eepromDataSize, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber)
  {
    auto ret = fns_.FT_EEPROM_Read(ftHandle, eepromData, eepromDataSize, Manufacturer, ManufacturerId, Description, SerialNumber);

    // TODO: decode program
    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\teepromData = 0x%p\n"
      "\teepromDataSize = %d\n"
      "\tManufacturer = %s\n"
      "\tManufacturerId = %s\n"
      "\tDescription = %s\n"
      "\tSerialNumber = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      eepromData,
      eepromDataSize,
      Manufacturer,
      ManufacturerId,
      Description,
      SerialNumber
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EEPROM_Program(FT_HANDLE ftHandle, void *eepromData, DWORD eepromDataSize, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_EEPROM_Program(ftHandle, eepromData, eepromDataSize, Manufacturer, ManufacturerId, Description, SerialNumber);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    // TODO: decode program
    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\teepromData = 0x%p\n"
      "\teepromDataSize = %d\n"
      "\tManufacturer = %s\n"
      "\tManufacturerId = %s\n"
      "\tDescription = %s\n"
      "\tSerialNumber = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      eepromData,
      eepromDataSize,
      Manufacturer,
      ManufacturerId,
      Description,
      SerialNumber
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetLatencyTimer(FT_HANDLE ftHandle, UCHAR ucLatency)
  {
    auto ret = fns_.FT_SetLatencyTimer(ftHandle, ucLatency);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tlatency = %" PRIu8,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      ucLatency
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetLatencyTimer(FT_HANDLE ftHandle, PUCHAR pucLatency)
  {
    auto ret = fns_.FT_GetLatencyTimer(ftHandle, pucLatency);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tlatency = %" PRIu8,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *pucLatency
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetBitMode(FT_HANDLE ftHandle, UCHAR ucMask, UCHAR ucEnable)
  {
    auto ret = fns_.FT_SetBitMode(ftHandle, ucMask, ucEnable);

    // TODO: decode mask and enable
    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tucMask = 0x%" PRIX8 "\n"
      "\tucEnable = 0x%" PRIX8,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      ucMask,
      ucEnable
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetBitMode(FT_HANDLE ftHandle, PUCHAR pucMode)
  {
    auto ret = fns_.FT_GetBitMode(ftHandle, pucMode);

    // TODO: decode mode
    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tmode = 0x%" PRIX8,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *pucMode
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetUSBParameters(FT_HANDLE ftHandle, ULONG ulInTransferSize, ULONG ulOutTransferSize)
  {
    auto ret = fns_.FT_SetUSBParameters(ftHandle, ulInTransferSize, ulOutTransferSize);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tinTransferSize = %ul\n"
      "\toutTransferSize = %ul",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      ulInTransferSize,
      ulOutTransferSize
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetDeadmanTimeout(FT_HANDLE ftHandle, ULONG ulDeadmanTimeout)
  {
    auto ret = fns_.FT_SetDeadmanTimeout(ftHandle, ulDeadmanTimeout);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tdeadmanTimeout = %ul",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      ulDeadmanTimeout
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetDeviceInfo(FT_HANDLE ftHandle, FT_DEVICE *lpftDevice, LPDWORD lpdwID, PCHAR SerialNumber, PCHAR Description, LPVOID Dummy)
  {
    auto ret = fns_.FT_GetDeviceInfo(ftHandle, lpftDevice, lpdwID, SerialNumber, Description, Dummy);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tftDevice = %s\n"
      "\tdwID = %d\n"
      "\tSerialNumber = %s\n"
      "\tDescription = %s\n"
      "\tDummy = 0x%p",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      FT_DEVICE_str(*lpftDevice).c_str(),
      *lpdwID,
      SerialNumber,
      Description,
      Dummy
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_StopInTask(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_StopInTask(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_RestartInTask(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_RestartInTask(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_SetResetPipeRetryCount(FT_HANDLE ftHandle, DWORD dwCount)
  {
    auto ret = fns_.FT_SetResetPipeRetryCount(ftHandle, dwCount);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tcount = %d",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      dwCount
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_ResetPort(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_ResetPort(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_CyclePort(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_CyclePort(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_HANDLE Ftd2xxSpy::FT_W32_CreateFile(LPCTSTR lpszName, DWORD dwAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreate, DWORD dwAttrsAndFlags, HANDLE hTemplate)
  {
    auto ret = fns_.FT_W32_CreateFile(lpszName, dwAccess, dwShareMode, lpSecurityAttributes, dwCreate, dwAttrsAndFlags, hTemplate);

    LPCVOID pArg1 = reinterpret_cast<LPCVOID>(lpszName);
    std::string pArg1_descr;
    std::string ret_descr;
    std::string descr;

    if (dwAttrsAndFlags & (FT_OPEN_BY_SERIAL_NUMBER | FT_OPEN_BY_DESCRIPTION))
    {
      pArg1_descr = fmt("\"%s\"", reinterpret_cast<const char *>(pArg1));
      if (dwAttrsAndFlags & FT_OPEN_BY_SERIAL_NUMBER)
        descr = fmt("serial:%s", pArg1_descr.c_str());
      else
        descr = fmt("descr:%s", pArg1_descr.c_str());
    }
    else if (dwAttrsAndFlags & FT_OPEN_BY_LOCATION)
    {
      pArg1_descr = fmt("%d", reinterpret_cast<DWORD>(pArg1));
      descr = fmt("locId:%s", pArg1_descr.c_str());
    }
    else
    {
      pArg1_descr = fmt("0x%p", pArg1);
      descr = fmt("unknown:0x%p", ret);
    }

    if (ret == INVALID_HANDLE_VALUE)
    {
      ret_descr = "INVALID_HANDLE_VALUE";
    }
    else
    {
      ret_descr = descr;
      StoreHandle(ret, descr);
    }

    logger.log(
      "Call %s() => %s\n"
      "\tpArg1 = %s\n"
      "\tdwAccess = %s\n"
      "\tdwShareMode = %d\n"
      "\tlpSecurityAttributes = 0x%p\n"
      "\tdwCreate = %s\n"
      "\tdwAttrsAndFlags = %s\n"
      "\thTemplate = 0x%p",
      __func__,
      ret_descr.c_str(),
      pArg1_descr.c_str(),
      FT_W32_CreateFile_Access_str(dwAccess).c_str(),
      dwShareMode,
      lpSecurityAttributes,
      FT_W32_CreateFile_Create_str(dwCreate).c_str(),
      FT_W32_CreateFile_AttrsAndFlags_str(dwAttrsAndFlags).c_str(),
      hTemplate
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_CloseHandle(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_W32_CloseHandle(ftHandle);

    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str()
    );

    if (ret)
      RemoveHandle(ftHandle);

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_ReadFile(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped)
  {
    auto ret = fns_.FT_W32_ReadFile(ftHandle, lpBuffer, nBufferSize, lpBytesReturned, lpOverlapped);

    // TODO: log data
    // TODO: log overlapped
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tlpBuffer = 0x%p\n"
      "\tnBufferSize = %d\n"
      "\tlpBytesReturned = %d\n"
      "\tlpOverlapped = 0x%p",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      lpBuffer,
      nBufferSize,
      *lpBytesReturned,
      lpOverlapped
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_WriteFile(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesWritten, LPOVERLAPPED lpOverlapped)
  {
    auto ret = cfg_.safe_mode ? TRUE : fns_.FT_W32_WriteFile(ftHandle, lpBuffer, nBufferSize, lpBytesWritten, lpOverlapped);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";
    // TODO: update overlapped in safe mode?

    // TODO: log data
    // TODO: log overlapped
    logger.log(
      "Call %s() => %d%s\n"
      "\thandle = %s\n"
      "\tlpBuffer = 0x%p\n"
      "\tnBufferSize = %d\n"
      "\tlpBytesWritten = %d\n"
      "\tlpOverlapped = 0x%p",
      __func__,
      ret,
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      lpBuffer,
      nBufferSize,
      *lpBytesWritten,
      lpOverlapped
    );

    return ret;
  }

  DWORD Ftd2xxSpy::FT_W32_GetLastError(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_W32_GetLastError(ftHandle);

    logger.log(
      "Call %s() => %d (%s)\n"
      "\thandle = %s",
      __func__,
      ret,
      FormatMessageA(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_GetOverlappedResult(FT_HANDLE ftHandle, LPOVERLAPPED lpOverlapped, LPDWORD lpdwBytesTransferred, BOOL bWait)
  {
    auto ret = fns_.FT_W32_GetOverlappedResult(ftHandle, lpOverlapped, lpdwBytesTransferred, bWait);

    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tlpOverlapped = 0x%p\n"
      "\tdwBytesTransferred = %d\n"
      "\tbWait = %d",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      lpOverlapped,
      *lpdwBytesTransferred,
      bWait
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_CancelIo(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_W32_CancelIo(ftHandle);

    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_ClearCommBreak(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_W32_ClearCommBreak(ftHandle);

    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_ClearCommError(FT_HANDLE ftHandle, LPDWORD lpdwErrors, LPFTCOMSTAT lpftComstat)
  {
    auto ret = fns_.FT_W32_ClearCommError(ftHandle, lpdwErrors, lpftComstat);

    // TODO: decode lpdwErrors
    // TODO: decode lpftComstat
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tlpdwErrors = 0x%" PRIX32 "\n"
      "\tlpftComstat = 0x%p",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      *lpdwErrors,
      lpftComstat
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_EscapeCommFunction(FT_HANDLE ftHandle, DWORD dwFunc)
  {
    auto ret = fns_.FT_W32_EscapeCommFunction(ftHandle, dwFunc);

    // TODO: decode dwFunc
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tdwFunc = 0x%" PRIX32,
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      dwFunc
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_GetCommModemStatus(FT_HANDLE ftHandle, LPDWORD lpdwModemStatus)
  {
    auto ret = fns_.FT_W32_GetCommModemStatus(ftHandle, lpdwModemStatus);

    // TODO: decode lpdwModemStatus
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tlpdwModemStatus = 0x%" PRIX32,
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      *lpdwModemStatus
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_GetCommState(FT_HANDLE ftHandle, LPFTDCB lpftDcb)
  {
    auto ret = fns_.FT_W32_GetCommState(ftHandle, lpftDcb);

    // TODO: decode lpftDcb
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tlpftDcb = 0x%p",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      lpftDcb
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_GetCommTimeouts(FT_HANDLE ftHandle, FTTIMEOUTS *pTimeouts)
  {
    auto ret = fns_.FT_W32_GetCommTimeouts(ftHandle, pTimeouts);

    // TODO: decode pTimeouts
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tpTimeouts = 0x%p",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      pTimeouts
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_PurgeComm(FT_HANDLE ftHandle, DWORD dwMask)
  {
    auto ret = fns_.FT_W32_PurgeComm(ftHandle, dwMask);

    // TODO: decode
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tdwMask = 0x%" PRIX32,
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      dwMask
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_SetCommBreak(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_W32_SetCommBreak(ftHandle);

    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_SetCommMask(FT_HANDLE ftHandle, ULONG ulEventMask)
  {
    auto ret = fns_.FT_W32_SetCommMask(ftHandle, ulEventMask);

    // TODO: decode ulEventMask
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "ulEventMask = 0x%" PRIX32,
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      ulEventMask
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_GetCommMask(FT_HANDLE ftHandle, LPDWORD lpdwEventMask)
  {
    auto ret = fns_.FT_W32_GetCommMask(ftHandle, lpdwEventMask);

    // TODO: decode lpdwEventMask
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tdwEventMask = 0x%" PRIX32,
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      *lpdwEventMask
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_SetCommState(FT_HANDLE ftHandle, LPFTDCB lpftDcb)
  {
    auto ret = fns_.FT_W32_SetCommState(ftHandle, lpftDcb);

    // TODO: decode lpftDcb
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tlpftDcb = 0x%p",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      lpftDcb
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_SetCommTimeouts(FT_HANDLE ftHandle, FTTIMEOUTS *pTimeouts)
  {
    auto ret = fns_.FT_W32_SetCommTimeouts(ftHandle, pTimeouts);

    // TODO: decode pTimeouts
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tpTimeouts = 0x%p",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      pTimeouts
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_SetupComm(FT_HANDLE ftHandle, DWORD dwReadBufferSize, DWORD dwWriteBufferSize)
  {
    auto ret = fns_.FT_W32_SetupComm(ftHandle, dwReadBufferSize, dwWriteBufferSize);

    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tdwReadBufferSize = %d\n"
      "\tdwWriteBufferSize = %d",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      dwReadBufferSize, dwWriteBufferSize
    );

    return ret;
  }

  BOOL Ftd2xxSpy::FT_W32_WaitCommEvent(FT_HANDLE ftHandle, PULONG pulEvent, LPOVERLAPPED lpOverlapped)
  {
    auto ret = fns_.FT_W32_WaitCommEvent(ftHandle, pulEvent, lpOverlapped);

    // TODO: decode pulEvent
    // TODO: decode lpOverlapped
    logger.log(
      "Call %s() => %d\n"
      "\thandle = %s\n"
      "\tulEvent = 0x% " PRIX32 "\n"
      "\tlpOverlapped = 0x%p",
      __func__,
      ret,
      ResolveHandle(ftHandle).c_str(),
      *pulEvent,
      lpOverlapped
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_CreateDeviceInfoList(LPDWORD lpdwNumDevs)
  {
    auto ret = fns_.FT_CreateDeviceInfoList(lpdwNumDevs);

    logger.log(
      "Call %s() => %s\n"
      "\tdwNumDevs = %d",
      __func__,
      FT_STATUS_str(ret).c_str(),
      *lpdwNumDevs
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetDeviceInfoList(FT_DEVICE_LIST_INFO_NODE *pDest, LPDWORD lpdwNumDevs)
  {
    auto ret = fns_.FT_GetDeviceInfoList(pDest, lpdwNumDevs);

    std::string pDest_descr;
    for (DWORD i = 0; i < *lpdwNumDevs; i++)
    {
      pDest_descr += fmt("\n\t\t%s", FT_DEVICE_LIST_INFO_NODE_str(&pDest[i]).c_str());
    }

    logger.log(
      "Call %s() => %s\n"
      "\tpDest = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      pDest_descr.c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetDeviceInfoDetail(DWORD dwIndex, LPDWORD lpdwFlags, LPDWORD lpdwType, LPDWORD lpdwID, LPDWORD lpdwLocId, LPVOID lpSerialNumber, LPVOID lpDescription, FT_HANDLE *pftHandle)
  {
    auto ret = fns_.FT_GetDeviceInfoDetail(dwIndex, lpdwFlags, lpdwType, lpdwID, lpdwLocId, lpSerialNumber, lpDescription, pftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\tdwIndex = %d\n"
      "\tdwFlags = 0x%X\n"
      "\tdwType = 0x%X\n"
      "\tdwID = %d\n"
      "\tdwLocId = %d\n"
      "\tserialNumber = %s\n"
      "\tdescription = %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      dwIndex,
      *lpdwFlags,
      *lpdwType,
      *lpdwID,
      *lpdwLocId,
      (char *)lpSerialNumber,
      (char *)lpDescription,
      ResolveHandle(pftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetDriverVersion(FT_HANDLE ftHandle, LPDWORD lpdwVersion)
  {
    auto ret = fns_.FT_GetDriverVersion(ftHandle, lpdwVersion);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tversion = 0x%" PRIX32,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *lpdwVersion
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetLibraryVersion(LPDWORD lpdwVersion)
  {
    auto ret = fns_.FT_GetLibraryVersion(lpdwVersion);

    logger.log(
      "Call %s() => %s\n"
      "\tversion = 0x%" PRIX32,
      __func__,
      FT_STATUS_str(ret).c_str(),
      *lpdwVersion
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_Rescan()
  {
    auto ret = fns_.FT_Rescan();

    logger.log(
      "Call %s() => %s",
      __func__,
      FT_STATUS_str(ret).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_Reload(WORD wVid, WORD wPid)
  {
    auto ret = fns_.FT_Reload(wVid, wPid);

    logger.log(
      "Call %s() => %s\n"
      "\twVid = 0x%" PRIX16 "\n"
      "\twPid = 0x%" PRIX16,
      __func__,
      FT_STATUS_str(ret).c_str(),
      wVid,
      wPid
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetComPortNumber(FT_HANDLE ftHandle, LPLONG lpdwComPortNumber)
  {
    auto ret = fns_.FT_GetComPortNumber(ftHandle, lpdwComPortNumber);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\tcomPortNumber = %d",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *lpdwComPortNumber
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EE_ReadConfig(FT_HANDLE ftHandle, UCHAR ucAddress, PUCHAR pucValue)
  {
    auto ret = fns_.FT_EE_ReadConfig(ftHandle, ucAddress, pucValue);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\taddress = 0x%" PRIX8 "\n"
      "\tvalue = 0x%" PRIX8,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      ucAddress,
      *pucValue
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EE_WriteConfig(FT_HANDLE ftHandle, UCHAR ucAddress, UCHAR ucValue)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_EE_WriteConfig(ftHandle, ucAddress, ucValue);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\taddress = 0x%" PRIX8 "\n"
      "\tvalue = 0x%" PRIX8,
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      ucAddress,
      ucValue
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_EE_ReadECC(FT_HANDLE ftHandle, UCHAR ucOption, LPWORD lpwValue)
  {
    auto ret = fns_.FT_EE_ReadECC(ftHandle, ucOption, lpwValue);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\toption = 0x%" PRIX8 "\n"
      "\tvalue = 0x%" PRIX8,
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      ucOption,
      *lpwValue
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_GetQueueStatusEx(FT_HANDLE ftHandle, DWORD *dwRxBytes)
  {
    auto ret = fns_.FT_GetQueueStatusEx(ftHandle, dwRxBytes);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s\n"
      "\trxBytes = %d",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str(),
      *dwRxBytes
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_ComPortIdle(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_ComPortIdle(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_ComPortCancelIdle(FT_HANDLE ftHandle)
  {
    auto ret = fns_.FT_ComPortCancelIdle(ftHandle);

    logger.log(
      "Call %s() => %s\n"
      "\thandle = %s",
      __func__,
      FT_STATUS_str(ret).c_str(),
      ResolveHandle(ftHandle).c_str()
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_VendorCmdGet(FT_HANDLE ftHandle, UCHAR Request, UCHAR *Buf, USHORT Len)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_VendorCmdGet(ftHandle, Request, Buf, Len);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    // TODO: log data
    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\trequest = 0x%" PRIX8 "\n"
      "\tbuf = 0x%p\n"
      "\tlen = %" PRIu16,
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      Request,
      Buf,
      Len
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_VendorCmdSet(FT_HANDLE ftHandle, UCHAR Request, UCHAR *Buf, USHORT Len)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_VendorCmdSet(ftHandle, Request, Buf, Len);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    // TODO: log data
    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\trequest = 0x%" PRIX8 "\n"
      "\tbuf = 0x%p\n"
      "\tlen = %" PRIu16,
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      Request,
      Buf,
      Len
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_VendorCmdGetEx(FT_HANDLE ftHandle, USHORT wValue, UCHAR *Buf, USHORT Len)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_VendorCmdGetEx(ftHandle, wValue, Buf, Len);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    // TODO: log data
    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\tvalue = 0x%" PRIX16 "\n"
      "\tbuf = 0x%p\n"
      "\tlen = %" PRIu16,
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      wValue,
      Buf,
      Len
    );

    return ret;
  }

  FT_STATUS Ftd2xxSpy::FT_VendorCmdSetEx(FT_HANDLE ftHandle, USHORT wValue, UCHAR *Buf, USHORT Len)
  {
    auto ret = cfg_.safe_mode ? FT_OK : fns_.FT_VendorCmdSetEx(ftHandle, wValue, Buf, Len);
    auto szSpylated = cfg_.safe_mode ? " (SPYLATED)" : "";

    // TODO: log data
    logger.log(
      "Call %s() => %s%s\n"
      "\thandle = %s\n"
      "\tvalue = 0x%" PRIX16 "\n"
      "\tbuf = 0x%p\n"
      "\tlen = %" PRIu16,
      __func__,
      FT_STATUS_str(ret).c_str(),
      szSpylated,
      ResolveHandle(ftHandle).c_str(),
      wValue,
      Buf,
      Len
    );

    return ret;
  }
}
