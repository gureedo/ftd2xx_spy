#include "ftd2xx_spy.h"
#include "util.h"

constexpr char kConfigPath[] = "./ftd2xx_spy.ini";

spy::Ftd2xxSpy g_spy;

std::string GetConfigFilePath()
{
  auto wstrPath = spy::GetModuleFileNameA();
  wstrPath.resize(wstrPath.find_last_of("\\"));
  return wstrPath + kConfigPath;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
    auto config_path = GetConfigFilePath();
    spy::Config config;
    config.Load(config_path.c_str());

    auto ret = g_spy.Init(config);
    if (!ret.first)
    {
      MessageBoxA(nullptr, ret.second.c_str(), "ftd2xx_spy", MB_ICONERROR);
      return FALSE;
    }
  }
  else if (fdwReason == DLL_PROCESS_DETACH)
  {
    g_spy.Close();
  }

  return TRUE;
}

extern "C" FTD2XX_API void WINAPI FT_Initialize(void)
{
  g_spy.FT_Initialize();
}

extern "C" FTD2XX_API void WINAPI FT_Finalise(void)
{
  g_spy.FT_Finalise();
}

FTD2XX_API FT_STATUS WINAPI FT_Open(int deviceNumber, FT_HANDLE *pHandle)
{
  return g_spy.FT_Open(deviceNumber, pHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_OpenEx(PVOID pArg1, DWORD Flags, FT_HANDLE *pHandle)
{
  return g_spy.FT_OpenEx(pArg1, Flags, pHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_ListDevices(PVOID pArg1, PVOID pArg2, DWORD Flags)
{
  return g_spy.FT_ListDevices(pArg1, pArg2, Flags);
}

FTD2XX_API FT_STATUS WINAPI FT_Close(FT_HANDLE ftHandle)
{
  return g_spy.FT_Close(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_Read(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToRead, LPDWORD lpBytesReturned)
{
  return g_spy.FT_Read(ftHandle, lpBuffer, dwBytesToRead, lpBytesReturned);
}

FTD2XX_API FT_STATUS WINAPI FT_Write(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToWrite, LPDWORD lpBytesWritten)
{
  return g_spy.FT_Write(ftHandle, lpBuffer, dwBytesToWrite, lpBytesWritten);
}

FTD2XX_API FT_STATUS WINAPI FT_IoCtl(FT_HANDLE ftHandle, DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped)
{
  return g_spy.FT_IoCtl(ftHandle, dwIoControlCode, lpInBuf, nInBufSize, lpOutBuf, nOutBufSize, lpBytesReturned, lpOverlapped);
}

FTD2XX_API FT_STATUS WINAPI FT_SetBaudRate(FT_HANDLE ftHandle, ULONG BaudRate)
{
  return g_spy.FT_SetBaudRate(ftHandle, BaudRate);
}

FTD2XX_API FT_STATUS WINAPI FT_SetDivisor(FT_HANDLE ftHandle, USHORT Divisor)
{
  return g_spy.FT_SetDivisor(ftHandle, Divisor);
}

FTD2XX_API FT_STATUS WINAPI FT_SetDataCharacteristics(FT_HANDLE ftHandle, UCHAR WordLength, UCHAR StopBits, UCHAR Parity)
{
  return g_spy.FT_SetDataCharacteristics(ftHandle, WordLength, StopBits, Parity);
}

FTD2XX_API FT_STATUS WINAPI FT_SetFlowControl(FT_HANDLE ftHandle, USHORT FlowControl, UCHAR XonChar, UCHAR XoffChar)
{
  return g_spy.FT_SetFlowControl(ftHandle, FlowControl, XonChar, XoffChar);
}

FTD2XX_API FT_STATUS WINAPI FT_ResetDevice(FT_HANDLE ftHandle)
{
  return g_spy.FT_ResetDevice(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_SetDtr(FT_HANDLE ftHandle)
{
  return g_spy.FT_SetDtr(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_ClrDtr(FT_HANDLE ftHandle)
{
  return g_spy.FT_ClrDtr(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_SetRts(FT_HANDLE ftHandle)
{
  return g_spy.FT_SetRts(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_ClrRts(FT_HANDLE ftHandle)
{
  return g_spy.FT_ClrRts(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_GetModemStatus(FT_HANDLE ftHandle, ULONG *pModemStatus)
{
  return g_spy.FT_GetModemStatus(ftHandle, pModemStatus);
}

FTD2XX_API FT_STATUS WINAPI FT_SetChars(FT_HANDLE ftHandle, UCHAR EventChar, UCHAR EventCharEnabled, UCHAR ErrorChar, UCHAR ErrorCharEnabled)
{
  return g_spy.FT_SetChars(ftHandle, EventChar, EventCharEnabled, ErrorChar, ErrorCharEnabled);
}

FTD2XX_API FT_STATUS WINAPI FT_Purge(FT_HANDLE ftHandle, ULONG Mask)
{
  return g_spy.FT_Purge(ftHandle, Mask);
}

FTD2XX_API FT_STATUS WINAPI FT_SetTimeouts(FT_HANDLE ftHandle, ULONG ReadTimeout, ULONG WriteTimeout)
{
  return g_spy.FT_SetTimeouts(ftHandle, ReadTimeout, WriteTimeout);
}

FTD2XX_API FT_STATUS WINAPI FT_GetQueueStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes)
{
  return g_spy.FT_GetQueueStatus(ftHandle, dwRxBytes);
}

FTD2XX_API FT_STATUS WINAPI FT_SetEventNotification(FT_HANDLE ftHandle, DWORD Mask, PVOID Param)
{
  return g_spy.FT_SetEventNotification(ftHandle, Mask, Param);
}

FTD2XX_API FT_STATUS WINAPI FT_GetStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes, DWORD *dwTxBytes, DWORD *dwEventDWord)
{
  return g_spy.FT_GetStatus(ftHandle, dwRxBytes, dwTxBytes, dwEventDWord);
}

FTD2XX_API FT_STATUS WINAPI FT_SetBreakOn(FT_HANDLE ftHandle)
{
  return g_spy.FT_SetBreakOn(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_SetBreakOff(FT_HANDLE ftHandle)
{
  return g_spy.FT_SetBreakOff(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_SetWaitMask(FT_HANDLE ftHandle, DWORD Mask)
{
  return g_spy.FT_SetWaitMask(ftHandle, Mask);
}

FTD2XX_API FT_STATUS WINAPI FT_WaitOnMask(FT_HANDLE ftHandle, DWORD *Mask)
{
  return g_spy.FT_WaitOnMask(ftHandle, Mask);
}

FTD2XX_API FT_STATUS WINAPI FT_GetEventStatus(FT_HANDLE ftHandle, DWORD *dwEventDWord)
{
  return g_spy.FT_GetEventStatus(ftHandle, dwEventDWord);
}

FTD2XX_API FT_STATUS WINAPI FT_ReadEE(FT_HANDLE ftHandle, DWORD dwWordOffset, LPWORD lpwValue)
{
  return g_spy.FT_ReadEE(ftHandle, dwWordOffset, lpwValue);
}

FTD2XX_API FT_STATUS WINAPI FT_WriteEE(FT_HANDLE ftHandle, DWORD dwWordOffset, WORD wValue)
{
  return g_spy.FT_WriteEE(ftHandle, dwWordOffset, wValue);
}

FTD2XX_API FT_STATUS WINAPI FT_EraseEE(FT_HANDLE ftHandle)
{
  return g_spy.FT_EraseEE(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_EE_Program(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData)
{
  return g_spy.FT_EE_Program(ftHandle, pData);
}

FTD2XX_API FT_STATUS WINAPI FT_EE_ProgramEx(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber)
{
  return g_spy.FT_EE_ProgramEx(ftHandle, pData, Manufacturer, ManufacturerId, Description, SerialNumber);
}

FTD2XX_API FT_STATUS WINAPI FT_EE_Read(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData)
{
  return g_spy.FT_EE_Read(ftHandle, pData);
}

FTD2XX_API FT_STATUS WINAPI FT_EE_ReadEx(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber)
{
  return g_spy.FT_EE_ReadEx(ftHandle, pData, Manufacturer, ManufacturerId, Description, SerialNumber);
}

FTD2XX_API FT_STATUS WINAPI FT_EE_UASize(FT_HANDLE ftHandle, LPDWORD lpdwSize)
{
  return g_spy.FT_EE_UASize(ftHandle, lpdwSize);
}

FTD2XX_API FT_STATUS WINAPI FT_EE_UAWrite(FT_HANDLE ftHandle, PUCHAR pucData, DWORD dwDataLen)
{
  return g_spy.FT_EE_UAWrite(ftHandle, pucData, dwDataLen);
}

FTD2XX_API FT_STATUS WINAPI FT_EE_UARead(FT_HANDLE ftHandle, PUCHAR pucData, DWORD dwDataLen, LPDWORD lpdwBytesRead)
{
  return g_spy.FT_EE_UARead(ftHandle, pucData, dwDataLen, lpdwBytesRead);
}

FTD2XX_API FT_STATUS WINAPI FT_EEPROM_Read(FT_HANDLE ftHandle, void *eepromData, DWORD eepromDataSize, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber)
{
  return g_spy.FT_EEPROM_Read(ftHandle, eepromData, eepromDataSize, Manufacturer, ManufacturerId, Description, SerialNumber);
}

FTD2XX_API FT_STATUS WINAPI FT_EEPROM_Program(FT_HANDLE ftHandle, void *eepromData, DWORD eepromDataSize, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber)
{
  return g_spy.FT_EEPROM_Program(ftHandle, eepromData, eepromDataSize, Manufacturer, ManufacturerId, Description, SerialNumber);
}

FTD2XX_API FT_STATUS WINAPI FT_SetLatencyTimer(FT_HANDLE ftHandle, UCHAR ucLatency)
{
  return g_spy.FT_SetLatencyTimer(ftHandle, ucLatency);
}

FTD2XX_API FT_STATUS WINAPI FT_GetLatencyTimer(FT_HANDLE ftHandle, PUCHAR pucLatency)
{
  return g_spy.FT_GetLatencyTimer(ftHandle, pucLatency);
}

FTD2XX_API FT_STATUS WINAPI FT_SetBitMode(FT_HANDLE ftHandle, UCHAR ucMask, UCHAR ucEnable)
{
  return g_spy.FT_SetBitMode(ftHandle, ucMask, ucEnable);
}

FTD2XX_API FT_STATUS WINAPI FT_GetBitMode(FT_HANDLE ftHandle, PUCHAR pucMode)
{
  return g_spy.FT_GetBitMode(ftHandle, pucMode);
}

FTD2XX_API FT_STATUS WINAPI FT_SetUSBParameters(FT_HANDLE ftHandle, ULONG ulInTransferSize, ULONG ulOutTransferSize)
{
  return g_spy.FT_SetUSBParameters(ftHandle, ulInTransferSize, ulOutTransferSize);
}

FTD2XX_API FT_STATUS WINAPI FT_SetDeadmanTimeout(FT_HANDLE ftHandle, ULONG ulDeadmanTimeout)
{
  return g_spy.FT_SetDeadmanTimeout(ftHandle, ulDeadmanTimeout);
}

FTD2XX_API FT_STATUS WINAPI FT_GetDeviceInfo(FT_HANDLE ftHandle, FT_DEVICE *lpftDevice, LPDWORD lpdwID, PCHAR SerialNumber, PCHAR Description, LPVOID Dummy)
{
  return g_spy.FT_GetDeviceInfo(ftHandle, lpftDevice, lpdwID, SerialNumber, Description, Dummy);
}

FTD2XX_API FT_STATUS WINAPI FT_StopInTask(FT_HANDLE ftHandle)
{
  return g_spy.FT_StopInTask(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_RestartInTask(FT_HANDLE ftHandle)
{
  return g_spy.FT_RestartInTask(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_SetResetPipeRetryCount(FT_HANDLE ftHandle, DWORD dwCount)
{
  return g_spy.FT_SetResetPipeRetryCount(ftHandle, dwCount);
}

FTD2XX_API FT_STATUS WINAPI FT_ResetPort(FT_HANDLE ftHandle)
{
  return g_spy.FT_ResetPort(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_CyclePort(FT_HANDLE ftHandle)
{
  return g_spy.FT_CyclePort(ftHandle);
}

FTD2XX_API FT_HANDLE WINAPI FT_W32_CreateFile(LPCTSTR lpszName, DWORD dwAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreate, DWORD dwAttrsAndFlags, HANDLE hTemplate)
{
  return g_spy.FT_W32_CreateFile(lpszName, dwAccess, dwShareMode, lpSecurityAttributes, dwCreate, dwAttrsAndFlags, hTemplate);
}

FTD2XX_API BOOL WINAPI FT_W32_CloseHandle(FT_HANDLE ftHandle)
{
  return g_spy.FT_W32_CloseHandle(ftHandle);
}

FTD2XX_API BOOL WINAPI FT_W32_ReadFile(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped)
{
  return g_spy.FT_W32_ReadFile(ftHandle, lpBuffer, nBufferSize, lpBytesReturned, lpOverlapped);
}

FTD2XX_API BOOL WINAPI FT_W32_WriteFile(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesWritten, LPOVERLAPPED lpOverlapped)
{
  return g_spy.FT_W32_WriteFile(ftHandle, lpBuffer, nBufferSize, lpBytesWritten, lpOverlapped);
}

FTD2XX_API DWORD WINAPI FT_W32_GetLastError(FT_HANDLE ftHandle)
{
  return g_spy.FT_W32_GetLastError(ftHandle);
}

FTD2XX_API BOOL WINAPI FT_W32_GetOverlappedResult(FT_HANDLE ftHandle, LPOVERLAPPED lpOverlapped, LPDWORD lpdwBytesTransferred, BOOL bWait)
{
  return g_spy.FT_W32_GetOverlappedResult(ftHandle, lpOverlapped, lpdwBytesTransferred, bWait);
}

FTD2XX_API BOOL WINAPI FT_W32_CancelIo(FT_HANDLE ftHandle)
{
  return g_spy.FT_W32_CancelIo(ftHandle);
}

FTD2XX_API BOOL WINAPI FT_W32_ClearCommBreak(FT_HANDLE ftHandle)
{
  return g_spy.FT_W32_ClearCommBreak(ftHandle);
}

FTD2XX_API BOOL WINAPI FT_W32_ClearCommError(FT_HANDLE ftHandle, LPDWORD lpdwErrors, LPFTCOMSTAT lpftComstat)
{
  return g_spy.FT_W32_ClearCommError(ftHandle, lpdwErrors, lpftComstat);
}

FTD2XX_API BOOL WINAPI FT_W32_EscapeCommFunction(FT_HANDLE ftHandle, DWORD dwFunc)
{
  return g_spy.FT_W32_EscapeCommFunction(ftHandle, dwFunc);
}

FTD2XX_API BOOL WINAPI FT_W32_GetCommModemStatus(FT_HANDLE ftHandle, LPDWORD lpdwModemStatus)
{
  return g_spy.FT_W32_GetCommModemStatus(ftHandle, lpdwModemStatus);
}

FTD2XX_API BOOL WINAPI FT_W32_GetCommState(FT_HANDLE ftHandle, LPFTDCB lpftDcb)
{
  return g_spy.FT_W32_GetCommState(ftHandle, lpftDcb);
}

FTD2XX_API BOOL WINAPI FT_W32_GetCommTimeouts(FT_HANDLE ftHandle, FTTIMEOUTS *pTimeouts)
{
  return g_spy.FT_W32_GetCommTimeouts(ftHandle, pTimeouts);
}

FTD2XX_API BOOL WINAPI FT_W32_PurgeComm(FT_HANDLE ftHandle, DWORD dwMask)
{
  return g_spy.FT_W32_PurgeComm(ftHandle, dwMask);
}

FTD2XX_API BOOL WINAPI FT_W32_SetCommBreak(FT_HANDLE ftHandle)
{
  return g_spy.FT_W32_SetCommBreak(ftHandle);
}

FTD2XX_API BOOL WINAPI FT_W32_SetCommMask(FT_HANDLE ftHandle, ULONG ulEventMask)
{
  return g_spy.FT_W32_SetCommMask(ftHandle, ulEventMask);
}

FTD2XX_API BOOL WINAPI FT_W32_GetCommMask(FT_HANDLE ftHandle, LPDWORD lpdwEventMask)
{
  return g_spy.FT_W32_GetCommMask(ftHandle, lpdwEventMask);
}

FTD2XX_API BOOL WINAPI FT_W32_SetCommState(FT_HANDLE ftHandle, LPFTDCB lpftDcb)
{
  return g_spy.FT_W32_SetCommState(ftHandle, lpftDcb);
}

FTD2XX_API BOOL WINAPI FT_W32_SetCommTimeouts(FT_HANDLE ftHandle, FTTIMEOUTS *pTimeouts)
{
  return g_spy.FT_W32_SetCommTimeouts(ftHandle, pTimeouts);
}

FTD2XX_API BOOL WINAPI FT_W32_SetupComm(FT_HANDLE ftHandle, DWORD dwReadBufferSize, DWORD dwWriteBufferSize)
{
  return g_spy.FT_W32_SetupComm(ftHandle, dwReadBufferSize, dwWriteBufferSize);
}

FTD2XX_API BOOL WINAPI FT_W32_WaitCommEvent(FT_HANDLE ftHandle, PULONG pulEvent, LPOVERLAPPED lpOverlapped)
{
  return g_spy.FT_W32_WaitCommEvent(ftHandle, pulEvent, lpOverlapped);
}

FTD2XX_API FT_STATUS WINAPI FT_CreateDeviceInfoList(LPDWORD lpdwNumDevs)
{
  return g_spy.FT_CreateDeviceInfoList(lpdwNumDevs);
}

FTD2XX_API FT_STATUS WINAPI FT_GetDeviceInfoList(FT_DEVICE_LIST_INFO_NODE *pDest, LPDWORD lpdwNumDevs)
{
  return g_spy.FT_GetDeviceInfoList(pDest, lpdwNumDevs);
}

FTD2XX_API FT_STATUS WINAPI FT_GetDeviceInfoDetail(DWORD dwIndex, LPDWORD lpdwFlags, LPDWORD lpdwType, LPDWORD lpdwID, LPDWORD lpdwLocId, LPVOID lpSerialNumber, LPVOID lpDescription, FT_HANDLE *pftHandle)
{
  return g_spy.FT_GetDeviceInfoDetail(dwIndex, lpdwFlags, lpdwType, lpdwID, lpdwLocId, lpSerialNumber, lpDescription, pftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_GetDriverVersion(FT_HANDLE ftHandle, LPDWORD lpdwVersion)
{
  return g_spy.FT_GetDriverVersion(ftHandle, lpdwVersion);
}

FTD2XX_API FT_STATUS WINAPI FT_GetLibraryVersion(LPDWORD lpdwVersion)
{
  return g_spy.FT_GetLibraryVersion(lpdwVersion);
}

FTD2XX_API FT_STATUS WINAPI FT_Rescan(void)
{
  return g_spy.FT_Rescan();
}

FTD2XX_API FT_STATUS WINAPI FT_Reload(WORD wVid, WORD wPid)
{
  return g_spy.FT_Reload(wVid, wPid);
}

FTD2XX_API FT_STATUS WINAPI FT_GetComPortNumber(FT_HANDLE ftHandle, LPLONG lpdwComPortNumber)
{
  return g_spy.FT_GetComPortNumber(ftHandle, lpdwComPortNumber);
}

FTD2XX_API FT_STATUS WINAPI FT_EE_ReadConfig(FT_HANDLE ftHandle, UCHAR ucAddress, PUCHAR pucValue)
{
  return g_spy.FT_EE_ReadConfig(ftHandle, ucAddress, pucValue);
}

FTD2XX_API FT_STATUS WINAPI FT_EE_WriteConfig(FT_HANDLE ftHandle, UCHAR ucAddress, UCHAR ucValue)
{
  return g_spy.FT_EE_WriteConfig(ftHandle, ucAddress, ucValue);
}

FTD2XX_API FT_STATUS WINAPI FT_EE_ReadECC(FT_HANDLE ftHandle, UCHAR ucOption, LPWORD lpwValue)
{
  return g_spy.FT_EE_ReadECC(ftHandle, ucOption, lpwValue);
}

FTD2XX_API FT_STATUS WINAPI FT_GetQueueStatusEx(FT_HANDLE ftHandle, DWORD *dwRxBytes)
{
  return g_spy.FT_GetQueueStatusEx(ftHandle, dwRxBytes);
}

FTD2XX_API FT_STATUS WINAPI FT_ComPortIdle(FT_HANDLE ftHandle)
{
  return g_spy.FT_ComPortIdle(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_ComPortCancelIdle(FT_HANDLE ftHandle)
{
  return g_spy.FT_ComPortCancelIdle(ftHandle);
}

FTD2XX_API FT_STATUS WINAPI FT_VendorCmdGet(FT_HANDLE ftHandle, UCHAR Request, UCHAR *Buf, USHORT Len)
{
  return g_spy.FT_VendorCmdGet(ftHandle, Request, Buf, Len);
}

FTD2XX_API FT_STATUS WINAPI FT_VendorCmdSet(FT_HANDLE ftHandle, UCHAR Request, UCHAR *Buf, USHORT Len)
{
  return g_spy.FT_VendorCmdSet(ftHandle, Request, Buf, Len);
}

FTD2XX_API FT_STATUS WINAPI FT_VendorCmdGetEx(FT_HANDLE ftHandle, USHORT wValue, UCHAR *Buf, USHORT Len)
{
  return g_spy.FT_VendorCmdGetEx(ftHandle, wValue, Buf, Len);
}

FTD2XX_API FT_STATUS WINAPI FT_VendorCmdSetEx(FT_HANDLE ftHandle, USHORT wValue, UCHAR *Buf, USHORT Len)
{
  return g_spy.FT_VendorCmdSetEx(ftHandle, wValue, Buf, Len);
}
