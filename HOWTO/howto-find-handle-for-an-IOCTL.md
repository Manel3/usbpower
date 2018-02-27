## Howto find handle for an IOCTL

DeviceIoControl need a handle (named hDevice) to determine the device.

Doc below tells to use the CreateFile function to retrieve it.

In fact usbdk does exactly this on UsbDk-1.00-19/UsbDkHelper/DriverFile.cpp:

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

TODO


## WINDOWS API:
DeviceIoControl function:
  https://msdn.microsoft.com/es-es/library/windows/desktop/aa363216(v=vs.85).aspx

Calling DeviceIoControl:
  https://msdn.microsoft.com/es-es/library/windows/desktop/aa363147(v=vs.85).aspx

Naming Files, Paths, and Namespaces:
  https://msdn.microsoft.com/es-es/library/windows/desktop/aa365247(v=vs.85).aspx
