## Howto find handle for an IOCTL

DeviceIoControl need a handle (named hDevice) to determine the device, how to get it?.

API doc tells to use the CreateFile function to retrieve it, we will need a device name string to call it.

## Key Windows API definitions and calls:
- call IOCTL: [DeviceIoControl](https://msdn.microsoft.com/es-es/library/windows/desktop/aa363216(v=vs.85).aspx)
    - [Calling DeviceIoControl](https://msdn.microsoft.com/es-es/library/windows/desktop/aa363147(v=vs.85).aspx)
- get handle: [CreateFile](https://msdn.microsoft.com/es-es/library/windows/desktop/aa363858(v=vs.85).aspx)
    - [Naming Files, Paths, and Namespaces](https://msdn.microsoft.com/es-es/library/windows/desktop/aa365247(v=vs.85).aspx)
    - USB external device filename, use **WinObj**: "\\?\USB#VID_XXXX&PID_XXXX#identifier_number#{GUID}" see below how to get it.
- existing USB IOCTL codes: [usbioctl.h](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/usbioctl/)
    - Note: IOCTL with **_INTERNAL_** on them can only be called from a driver, and not from user space

### USB device ID to external device name

- device ID = USB\VID_05E3&PID_0608\5&147EFEE4&0&3
- external device filename = \\\\?\\USB#VID_05E3&PID_0608#5&147EFEE4&0&3#{f18a0e88-c30c-11d0-8815-00a0c906bed8}

### To find device ID using devcon.exe

install Windows Software Development Kit
Put C:\Program Files (x86)\Windows Kits\10\Tools\x64\devcon.exe in the path (or copy devcon.exe to C:\Windows)

```c
# show all USB devices
devcon listclass usb
USB\VID_05E3&PID_0608\5&147EFEE4&0&3

# show all device names ordered
devcon find * | sort
```

### To find device ID from device manager

On Win10 device manager > properties > Events tab:
Se configuró el dispositivo (Configured device) USB\VID_05E3&PID_0608\5&147efee4&0&3.

On Win10 device manager > properties > Details tab > select "access path to device instance":
USB\VID_05E3&PID_0608\5&147efee4&0&3


## To find external device filename: WinObj USB HUB

To find external "user space" device filename:

Download WinObj v2.22
- https://docs.microsoft.com/en-us/sysinternals/downloads/winobj

In WinObj go to: **\\** > **GLOBAL??**
![WinObj](https://github.com/manel3/usbpower/HOWTO/WinObj.png "WinObj")

Determing the correct symbolic link to pass to CreateFile
- https://community.microfocus.com/borland/develop/devpartner_-_code_analysis/w/knowledge_base/5190/determing-the-correct-symbolic-link-to-pass-to-createfile

Open device name using CreateFile
- https://stackoverflow.com/questions/21703592/open-device-name-using-createfile

How to use CreateFile to open a handle to an external USB device
- https://social.msdn.microsoft.com/Forums/windowsdesktop/en-US/82f91870-0050-4202-8aaa-f9e4b62421c9/how-to-use-createfile-to-open-a-handle-to-an-external-usb-device?forum=wdk


## TESTING IT:
Lets try to comunicate with USB HUB with existing IOCTL:
- [IOCTL_USB_GET_HUB_CAPABILITIES_EX](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/usbioctl/ni-usbioctl-ioctl_usb_get_hub_capabilities_ex)
- And also with a not existing IOCTL to test the error return

[test-ioctl-to-hub.c](https://github.com/Manel3/usbpower/blob/master/TESTS/test-ioctl-to-hub.c)
``` c
D:\usbpower\TESTS>test-ioctl-to-hub.exe "\\?\USB#VID_05E3&PID_0608#5&147EFEE4&0&3#{f18a0e88-c30c-11d0-8815-00a0c906bed8}"
Opened device: \\?\USB#VID_05E3&PID_0608#5&147EFEE4&0&3#{f18a0e88-c30c-11d0-8815-00a0c906bed8}
DeviceIoControl IOCTL_USB_GET_HUB_CAPABILITIES_EX success
DeviceIoControl IOCTL_UNKNOWN success
```

### How it is done on:

usbdk project **DeviceIoControl** on UsbDk-1.00-19/UsbDkHelper/DriverFile.cpp

``` c
TransferResult UsbDkDriverFile::Ioctl(DWORD Code,
                            bool ShortBufferOk,
                            LPVOID InBuffer,
                            DWORD InBufferSize,
                            LPVOID OutBuffer,
                            DWORD OutBufferSize,
                            LPDWORD BytesReturned,
                            LPOVERLAPPED Overlapped)
{
    DWORD InternalBytesReturned;
    LPDWORD InternalBytesReturnedPtr = (BytesReturned != nullptr) ? BytesReturned : &InternalBytesReturned;
    if (!DeviceIoControl(m_hDriver, Code,
                         InBuffer, InBufferSize,
                         OutBuffer, OutBufferSize,
                         InternalBytesReturnedPtr, Overlapped))
    {
        auto err = GetLastError();
        if (m_bOverlapped && (err == ERROR_IO_PENDING))
        {
            // If driver was open without FILE_FLAG_OVERLAPPED, DeviceIoControl can't return ERROR_IO_PENDING,
            // so the caller of Ioctl can check return result as boolean
            return TransferSuccessAsync;
        }
        if (ShortBufferOk && (err == ERROR_MORE_DATA))
        {
            return TransferFailure;
        }

        throw UsbDkDriverFileException(TEXT("DeviceIoControl failed"));
    }

    return TransferSuccess;
}
```

In fact usbdk project uses CreateFile on UsbDk-1.00-19/UsbDkHelper/DriverFile.cpp

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

