## Howto find handle for an IOCTL

DeviceIoControl need a handle (named hDevice) to determine the device.

Doc below tells to use the CreateFile function to retrieve it.

In fact usbdk project does exactly this on UsbDk-1.00-19/UsbDkHelper/DriverFile.cpp:

``` c
UsbDkDriverFile::UsbDkDriverFile(LPCTSTR lpFileName, bool bOverlapped)
{
    m_bOverlapped = bOverlapped;

    m_hDriver = CreateFile(lpFileName,
                           GENERIC_READ | GENERIC_WRITE,
                           0,
                           nullptr,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL | (bOverlapped ? FILE_FLAG_OVERLAPPED : 0),
                           nullptr);

    if (m_hDriver == INVALID_HANDLE_VALUE)
    {
        throw UsbDkDriverFileException(tstring(TEXT("Failed to open device symlink ")) + lpFileName);
    }
}
```
Lets see how must be created **lpFileName** 

In usbdk project it comes from: 
- UsbDkDriverFile(LPCTSTR lpFileName, bool bOverlapped = false);
- class UsbDkRedirectorAccess : public UsbDkDriverFile (in RedirectorAccess.h)
- (indirectly from:) HANDLE UsbDk_StartRedirect(PUSB_DK_DEVICE_ID DeviceID)
- static int Controller_RedirectDevice(TCHAR *DeviceID, TCHAR *InstanceID)
- int __cdecl _tmain(int argc, TCHAR* argv[]) when called with parameter -r in: Controller_RedirectDevice(argv[2], argv[3]);

Help of **UsbDkController -r ID SN** tells to use "USB\\VID_XXXX&PID_XXXX"

TEST THAT

## WINDOWS API:
DeviceIoControl function:
  https://msdn.microsoft.com/es-es/library/windows/desktop/aa363216(v=vs.85).aspx

Calling DeviceIoControl:
  https://msdn.microsoft.com/es-es/library/windows/desktop/aa363147(v=vs.85).aspx

Naming Files, Paths, and Namespaces:
  https://msdn.microsoft.com/es-es/library/windows/desktop/aa365247(v=vs.85).aspx
