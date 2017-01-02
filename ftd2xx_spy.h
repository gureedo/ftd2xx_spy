#pragma once

#include <string>
#include <unordered_map>

#include "config.h"
#include "ftd2xx_shim.h"
#include "logger.h"


namespace spy {
  class Ftd2xxSpy
  {
  public:
    Ftd2xxSpy() = default;
    Ftd2xxSpy(Ftd2xxSpy &&other) = delete;
    Ftd2xxSpy(const Ftd2xxSpy &other) = delete;
    Ftd2xxSpy &operator=(Ftd2xxSpy &&other) = delete;
    Ftd2xxSpy &operator=(const Ftd2xxSpy &other) = delete;
    ~Ftd2xxSpy();

    std::pair<bool, std::string> Init(Config &config);
    void Close();

    void StoreHandle(FT_HANDLE handle, std::string &descr);
    void RemoveHandle(FT_HANDLE handle);
    std::string ResolveHandle(FT_HANDLE handle);

    /*
     * FT2XX FUNCTIONS
     */

    void FT_Initialize();
    void FT_Finalise();
    FT_STATUS FT_Open(int deviceNumber, FT_HANDLE *pHandle);
    FT_STATUS FT_OpenEx(PVOID pArg1, DWORD Flags, FT_HANDLE *pHandle);
    FT_STATUS FT_ListDevices(PVOID pArg1, PVOID pArg2, DWORD Flags);
    FT_STATUS FT_Close(FT_HANDLE ftHandle);
    FT_STATUS FT_Read(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToRead, LPDWORD lpBytesReturned);
    FT_STATUS FT_Write(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToWrite, LPDWORD lpBytesWritten);
    FT_STATUS FT_IoCtl(FT_HANDLE ftHandle, DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
    FT_STATUS FT_SetBaudRate(FT_HANDLE ftHandle, ULONG BaudRate);
    FT_STATUS FT_SetDivisor(FT_HANDLE ftHandle, USHORT Divisor);
    FT_STATUS FT_SetDataCharacteristics(FT_HANDLE ftHandle, UCHAR WordLength, UCHAR StopBits, UCHAR Parity);
    FT_STATUS FT_SetFlowControl(FT_HANDLE ftHandle, USHORT FlowControl, UCHAR XonChar, UCHAR XoffChar);
    FT_STATUS FT_ResetDevice(FT_HANDLE ftHandle);
    FT_STATUS FT_SetDtr(FT_HANDLE ftHandle);
    FT_STATUS FT_ClrDtr(FT_HANDLE ftHandle);
    FT_STATUS FT_SetRts(FT_HANDLE ftHandle);
    FT_STATUS FT_ClrRts(FT_HANDLE ftHandle);
    FT_STATUS FT_GetModemStatus(FT_HANDLE ftHandle, ULONG *pModemStatus);
    FT_STATUS FT_SetChars(FT_HANDLE ftHandle, UCHAR EventChar, UCHAR EventCharEnabled, UCHAR ErrorChar, UCHAR ErrorCharEnabled);
    FT_STATUS FT_Purge(FT_HANDLE ftHandle, ULONG Mask);
    FT_STATUS FT_SetTimeouts(FT_HANDLE ftHandle, ULONG ReadTimeout, ULONG WriteTimeout);
    FT_STATUS FT_GetQueueStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes);
    FT_STATUS FT_SetEventNotification(FT_HANDLE ftHandle, DWORD Mask, PVOID Param);
    FT_STATUS FT_GetStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes, DWORD *dwTxBytes, DWORD *dwEventDWord);
    FT_STATUS FT_SetBreakOn(FT_HANDLE ftHandle);
    FT_STATUS FT_SetBreakOff(FT_HANDLE ftHandle);
    FT_STATUS FT_SetWaitMask(FT_HANDLE ftHandle, DWORD Mask);
    FT_STATUS FT_WaitOnMask(FT_HANDLE ftHandle, DWORD *Mask);
    FT_STATUS FT_GetEventStatus(FT_HANDLE ftHandle, DWORD *dwEventDWord);
    FT_STATUS FT_ReadEE(FT_HANDLE ftHandle, DWORD dwWordOffset, LPWORD lpwValue);
    FT_STATUS FT_WriteEE(FT_HANDLE ftHandle, DWORD dwWordOffset, WORD wValue);
    FT_STATUS FT_EraseEE(FT_HANDLE ftHandle);
    FT_STATUS FT_EE_Program(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData);
    FT_STATUS FT_EE_ProgramEx(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber);
    FT_STATUS FT_EE_Read(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData);
    FT_STATUS FT_EE_ReadEx(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber);
    FT_STATUS FT_EE_UASize(FT_HANDLE ftHandle, LPDWORD lpdwSize);
    FT_STATUS FT_EE_UAWrite(FT_HANDLE ftHandle, PUCHAR pucData, DWORD dwDataLen);
    FT_STATUS FT_EE_UARead(FT_HANDLE ftHandle, PUCHAR pucData, DWORD dwDataLen, LPDWORD lpdwBytesRead);
    FT_STATUS FT_EEPROM_Read(FT_HANDLE ftHandle, void *eepromData, DWORD eepromDataSize, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber);
    FT_STATUS FT_EEPROM_Program(FT_HANDLE ftHandle, void *eepromData, DWORD eepromDataSize, char *Manufacturer, char *ManufacturerId, char *Description, char *SerialNumber);
    FT_STATUS FT_SetLatencyTimer(FT_HANDLE ftHandle, UCHAR ucLatency);
    FT_STATUS FT_GetLatencyTimer(FT_HANDLE ftHandle, PUCHAR pucLatency);
    FT_STATUS FT_SetBitMode(FT_HANDLE ftHandle, UCHAR ucMask, UCHAR ucEnable);
    FT_STATUS FT_GetBitMode(FT_HANDLE ftHandle, PUCHAR pucMode);
    FT_STATUS FT_SetUSBParameters(FT_HANDLE ftHandle, ULONG ulInTransferSize, ULONG ulOutTransferSize);
    FT_STATUS FT_SetDeadmanTimeout(FT_HANDLE ftHandle, ULONG ulDeadmanTimeout);
    FT_STATUS FT_GetDeviceInfo(FT_HANDLE ftHandle, FT_DEVICE *lpftDevice, LPDWORD lpdwID, PCHAR SerialNumber, PCHAR Description, LPVOID Dummy);
    FT_STATUS FT_StopInTask(FT_HANDLE ftHandle);
    FT_STATUS FT_RestartInTask(FT_HANDLE ftHandle);
    FT_STATUS FT_SetResetPipeRetryCount(FT_HANDLE ftHandle, DWORD dwCount);
    FT_STATUS FT_ResetPort(FT_HANDLE ftHandle);
    FT_STATUS FT_CyclePort(FT_HANDLE ftHandle);
    FT_HANDLE FT_W32_CreateFile(LPCTSTR lpszName, DWORD dwAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreate, DWORD dwAttrsAndFlags, HANDLE hTemplate);
    BOOL FT_W32_CloseHandle(FT_HANDLE ftHandle);
    BOOL FT_W32_ReadFile(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
    BOOL FT_W32_WriteFile(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesWritten, LPOVERLAPPED lpOverlapped);
    DWORD FT_W32_GetLastError(FT_HANDLE ftHandle);
    BOOL FT_W32_GetOverlappedResult(FT_HANDLE ftHandle, LPOVERLAPPED lpOverlapped, LPDWORD lpdwBytesTransferred, BOOL bWait);
    BOOL FT_W32_CancelIo(FT_HANDLE ftHandle);
    BOOL FT_W32_ClearCommBreak(FT_HANDLE ftHandle);
    BOOL FT_W32_ClearCommError(FT_HANDLE ftHandle, LPDWORD lpdwErrors, LPFTCOMSTAT lpftComstat);
    BOOL FT_W32_EscapeCommFunction(FT_HANDLE ftHandle, DWORD dwFunc);
    BOOL FT_W32_GetCommModemStatus(FT_HANDLE ftHandle, LPDWORD lpdwModemStatus);
    BOOL FT_W32_GetCommState(FT_HANDLE ftHandle, LPFTDCB lpftDcb);
    BOOL FT_W32_GetCommTimeouts(FT_HANDLE ftHandle, FTTIMEOUTS *pTimeouts);
    BOOL FT_W32_PurgeComm(FT_HANDLE ftHandle, DWORD dwMask);
    BOOL FT_W32_SetCommBreak(FT_HANDLE ftHandle);
    BOOL FT_W32_SetCommMask(FT_HANDLE ftHandle, ULONG ulEventMask);
    BOOL FT_W32_GetCommMask(FT_HANDLE ftHandle, LPDWORD lpdwEventMask);
    BOOL FT_W32_SetCommState(FT_HANDLE ftHandle, LPFTDCB lpftDcb);
    BOOL FT_W32_SetCommTimeouts(FT_HANDLE ftHandle, FTTIMEOUTS *pTimeouts);
    BOOL FT_W32_SetupComm(FT_HANDLE ftHandle, DWORD dwReadBufferSize, DWORD dwWriteBufferSize);
    BOOL FT_W32_WaitCommEvent(FT_HANDLE ftHandle, PULONG pulEvent, LPOVERLAPPED lpOverlapped);
    FT_STATUS FT_CreateDeviceInfoList(LPDWORD lpdwNumDevs);
    FT_STATUS FT_GetDeviceInfoList(FT_DEVICE_LIST_INFO_NODE *pDest, LPDWORD lpdwNumDevs);
    FT_STATUS FT_GetDeviceInfoDetail(DWORD dwIndex, LPDWORD lpdwFlags, LPDWORD lpdwType, LPDWORD lpdwID, LPDWORD lpdwLocId, LPVOID lpSerialNumber, LPVOID lpDescription, FT_HANDLE *pftHandle);
    FT_STATUS FT_GetDriverVersion(FT_HANDLE ftHandle, LPDWORD lpdwVersion);
    FT_STATUS FT_GetLibraryVersion(LPDWORD lpdwVersion);
    FT_STATUS FT_Rescan();
    FT_STATUS FT_Reload(WORD wVid, WORD wPid);
    FT_STATUS FT_GetComPortNumber(FT_HANDLE ftHandle, LPLONG	lpdwComPortNumber);
    FT_STATUS FT_EE_ReadConfig(FT_HANDLE ftHandle, UCHAR ucAddress, PUCHAR pucValue);
    FT_STATUS FT_EE_WriteConfig(FT_HANDLE ftHandle, UCHAR ucAddress, UCHAR ucValue);
    FT_STATUS FT_EE_ReadECC(FT_HANDLE ftHandle, UCHAR ucOption, LPWORD lpwValue);
    FT_STATUS FT_GetQueueStatusEx(FT_HANDLE ftHandle, DWORD *dwRxBytes);
    FT_STATUS FT_ComPortIdle(FT_HANDLE ftHandle);
    FT_STATUS FT_ComPortCancelIdle(FT_HANDLE ftHandle);
    FT_STATUS FT_VendorCmdGet(FT_HANDLE ftHandle, UCHAR Request, UCHAR *Buf, USHORT Len);
    FT_STATUS FT_VendorCmdSet(FT_HANDLE ftHandle, UCHAR Request, UCHAR *Buf, USHORT Len);
    FT_STATUS FT_VendorCmdGetEx(FT_HANDLE ftHandle, USHORT wValue, UCHAR *Buf, USHORT Len);
    FT_STATUS FT_VendorCmdSetEx(FT_HANDLE ftHandle, USHORT wValue, UCHAR *Buf, USHORT Len);

  protected:
    void LoadConfig(const char *file_path);
    static std::string FT_STATUS_str(FT_STATUS status);
    static std::string FT_ListDevices_Flags_str(DWORD flags);
    static std::string FT_OpenEx_Flags_str(DWORD flags);
    static std::string FT_PARITY_str(UCHAR parity);
    static std::string FT_BITS_str(UCHAR bits);
    static std::string FT_STOP_BITS_str(UCHAR bits);
    static std::string FT_FLOW_str(USHORT flow);
    static std::string FT_PURGE_str(ULONG mask);
    static std::string FT_EVENT_str(DWORD mask);
    static std::string FT_DEVICE_str(FT_DEVICE device);
    static std::string FT_W32_CreateFile_Access_str(DWORD dwAccess);
    static std::string FT_W32_CreateFile_Create_str(DWORD dwCreate);
    static std::string FT_W32_CreateFile_AttrsAndFlags_str(DWORD dwAttrsAndFlags);
    std::string FT_DEVICE_LIST_INFO_NODE_str(const FT_DEVICE_LIST_INFO_NODE *node);

  private:
    HMODULE dll_handle_{ nullptr };
    ftd2xx_functions_t fns_;
    Logger logger;
    std::unordered_map<FT_HANDLE, std::string> handles_;
    Config cfg_;
  };
};
